
# sources
FILE(GLOB CS_SIMCONTROL_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# headers
FILE(GLOB CS_SIMCONTROL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

#设置自动生成的头文件到src目录
SET(CS_EXPORT_HEADER ${CMAKE_SOURCE_DIR}/spliceForTool/simControl/simControl_export.h)
#设置工程名称
SET(CURRENT_PROJ_NAME simControl)

SET(CS_SIMCONTROL_SRC ${CS_SIMCONTROL_SRC} ${CS_SIMCONTROL_HEADERS} )

SET(CS_SIMCONTROL_HEADERS ${CS_SIMCONTROL_HEADERS} ${CS_EXPORT_HEADER})

#包含目录
INCLUDE_DIRECTORIES(
					${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include
					${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include/dcps/C++/isocpp2
					${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include/sys
					${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include/dcps/C++/SACPP
					${CMAKE_SOURCE_DIR}/spliceForTool/external
					${CMAKE_SOURCE_DIR}/spliceForTool/external/rapidxml
					${CMAKE_SOURCE_DIR}/spliceForTool/cssimlog
					${CMAKE_SOURCE_DIR}/spliceForTool/dds/csdds_msg_type
					${CMAKE_SOURCE_DIR}/spliceForTool/dds/dds_service					
)

#库目录
LINK_DIRECTORIES( 
				${CMAKE_LIBRARY_OUTPUT_DIRECTORY} 
				${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/lib
				)

################################################################################################${CS_BROWSER_MOC_SRCS}

LINK_LIBRARIES(
  cssimlog
  dcpsisocpp2.lib
  ddskernel.lib
)

#生成库
ADD_LIBRARY(${CURRENT_PROJ_NAME} SHARED ${CS_SIMCONTROL_HEADERS} ${CS_SIMCONTROL_SRC})

#SET_TARGET_PROPERTIES(${CURRENT_PROJ_NAME} PROPERTIES FOLDER "common")

#生成头文件
GENERATE_EXPORT_HEADER(simControl
EXPORT_FILE_NAME ${CS_EXPORT_HEADER}
)

#链接库
 TARGET_LINK_LIBRARIES(simControl  
	cssimlog
	dcpsisocpp2
	ddskernel
	#DemoTypes
	csdds_msg_type
	csdds_service
 )

