
# sources
FILE(GLOB CS_XX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# headers
FILE(GLOB CS_XX_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

#设置自动生成的头文件到src目录
SET(CS_EXPORT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/csmongomgr_export.h)

#设置工程名称
SET(CURRENT_PROJ_NAME csmongomgr)

SET(CS_XX_HEADERS ${CS_XX_HEADERS} ${CS_EXPORT_HEADER})

SET(CS_XX_SRC ${CS_XX_SRC} ${CS_XX_HEADERS})

#包含目录
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
					#${CMAKE_SOURCE_DIR}/src/common
				   	${CMAKE_SOURCE_DIR}/spliceForTool/dds
					#${CMAKE_SOURCE_DIR}/src/engine
					${CMAKE_SOURCE_DIR}/spliceForTool/external/mongo-cxx-driver-r3.4.0/include/mongocxx/v_noabi
					${CMAKE_SOURCE_DIR}/spliceForTool/external/mongo-cxx-driver-r3.4.0/include/bsoncxx/v_noabi
					${CMAKE_SOURCE_DIR}/spliceForTool/external/mongo-cxx-driver-r3.4.0/include
					${CMAKE_SOURCE_DIR}/spliceForTool/external/boost-1_70/include
)

#库目录
LINK_DIRECTORIES( ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} 
                  ${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/lib
				  ${CMAKE_SOURCE_DIR}/spliceForTool/external/mongo-cxx-driver-r3.4.0/lib
				)

#链接库TARGET
LINK_LIBRARIES(       
	bsoncxx
	mongocxx
	#csstructwrap
	#csdata_types
)				

#生成库
ADD_LIBRARY(${CURRENT_PROJ_NAME} SHARED ${CS_XX_HEADERS} ${CS_XX_SRC})

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
#  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
#        GROUP_EXECUTE GROUP_READ
#  )