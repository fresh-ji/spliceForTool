
# sources
FILE(GLOB CS_DDS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# headers
FILE(GLOB CS_DDS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

#设置自动生成的头文件到src目录
SET(CS_EXPORT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/csdds_service_export.h)

#设置工程名称
SET(CURRENT_PROJ_NAME csdds_service)

SET(CS_DDS_SRC ${CS_DDS_SRC} ${CMAKE_SOURCE_DIR}/spliceForTool/dds/check_status/check_status.cpp) 
SET(CS_DDS_HEADERS ${CS_DDS_HEADERS} ${CS_EXPORT_HEADER} ${CMAKE_SOURCE_DIR}/spliceForTool/dds/check_status/check_status.h)

SET(CS_DDS_SRC ${CS_DDS_SRC} ${CS_DDS_HEADERS})

#包含目录
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
				    ${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include
					${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include/dcps/C++/isocpp2
					${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include/sys
					${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include/dcps/C++/SACPP
					${CMAKE_SOURCE_DIR}/spliceForTool/dds/check_status
					${CMAKE_SOURCE_DIR}/spliceForTool/dds/dds_msg_types
					${CMAKE_SOURCE_DIR}/spliceForTool/common
					${CMAKE_SOURCE_DIR}/spliceForTool
)

#库目录
LINK_DIRECTORIES( ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} 
                  ${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/lib 				  
				)

#链接库TARGET_
LINK_LIBRARIES(   
   ddskernel.lib
   dcpssacpp.lib
   csdds_msg_type.lib
   cssimlog
)				

#生成库
ADD_LIBRARY(${CURRENT_PROJ_NAME} SHARED ${CS_DDS_HEADERS} ${CS_DDS_SRC})

SET_TARGET_PROPERTIES(${CURRENT_PROJ_NAME} PROPERTIES FOLDER "dds")

#生成头文件
GENERATE_EXPORT_HEADER(${CURRENT_PROJ_NAME}
EXPORT_FILE_NAME ${CS_EXPORT_HEADER}
)

#INSTALL()
#INSTALL(TARGETS ${CURRENT_PROJ_NAME}
#  RUNTIME DESTINATION ${12YSIM_INSTALL_PATH}
#  LIBRARY DESTINATION ${12YSIM_INSTALL_PATH}
#  ARCHIVE DESTINATION ${12YSIM_INSTALL_PATH}
#  FRAMEWORK DESTINATION ${12YSIM_INSTALL_PATH}
#  PUBLIC_HEADER DESTINATION ${12YSIM_INSTALL_PATH}
#  )

#INSTALL(FILES ${CMAKE_RUNTIME_RELEASE_OUTPUT_DIRECTORY}/${CURRENT_PROJ_NAME}.pdb DESTINATION ./pdb
  # PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
        # GROUP_EXECUTE GROUP_READ
  # )