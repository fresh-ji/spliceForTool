
# sources
FILE(GLOB CS_CONTROLTEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# headers
FILE(GLOB CS_CONTROLTEST_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

#设置自动生成的头文件到src目录
#SET(CS_EXPORT_HEADER #${CMAKE_SOURCE_DIR}/spliceForTool/spliceForTool/spliceForTool_export.h)

#设置工程名称
SET(CURRENT_PROJ_NAME controlTest)

SET(CS_CONTROLTEST_SRC ${CS_CONTROLTEST_SRC} ${CS_CONTROLTEST_HEADERS} )

#SET(CS_CONTROLTEST_HEADERS ${CS_CONTROLTEST_HEADERS} ${CS_EXPORT_HEADER})

#包含目录
INCLUDE_DIRECTORIES(
				
)

#库目录
LINK_DIRECTORIES( 
				${CMAKE_LIBRARY_OUTPUT_DIRECTORY} 
				${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/lib
				${CMAKE_SOURCE_DIR}/spliceForTool/external/ospl
)

################################################################################################${CS_BROWSER_MOC_SRCS}

#生成库
ADD_EXECUTABLE(${CURRENT_PROJ_NAME}  ${CS_CONTROLTEST_HEADERS} ${CS_CONTROLTEST_SRC})

SET_TARGET_PROPERTIES(${CURRENT_PROJ_NAME} PROPERTIES FOLDER "test")

#生成头文件
#GENERATE_EXPORT_HEADER(toolTestA
#EXPORT_FILE_NAME ${CS_EXPORT_HEADER}
#)

#链接库
TARGET_LINK_LIBRARIES(controlTest  
  cssimlog
  #simControl
  #ddskernel
  #dcpsisocpp2
  #DemoTypes
  #csdds_msg_type
  #csdds_service
)

