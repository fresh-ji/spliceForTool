
# sources
FILE(GLOB CS_TOOLTESTC_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# headers
FILE(GLOB CS_TOOLTESTC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

#设置自动生成的头文件到src目录
#SET(CS_EXPORT_HEADER #${CMAKE_SOURCE_DIR}/spliceForTool/spliceForTool/spliceForTool_export.h)

#设置工程名称
SET(CURRENT_PROJ_NAME toolTestC)

SET(CS_TOOLTESTC_SRC ${CS_TOOLTESTC_SRC} ${CS_TOOLTESTC_HEADERS} )

#SET(CS_TOOLTESTC_HEADERS ${CS_TOOLTESTC_HEADERS} ${CS_EXPORT_HEADER})

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
ADD_EXECUTABLE(${CURRENT_PROJ_NAME}  ${CS_TOOLTESTC_HEADERS} ${CS_TOOLTESTC_SRC})

SET_TARGET_PROPERTIES(${CURRENT_PROJ_NAME} PROPERTIES FOLDER "test")

#生成头文件
#GENERATE_EXPORT_HEADER(toolTestC
#EXPORT_FILE_NAME ${CS_EXPORT_HEADER}
#)

#链接库
TARGET_LINK_LIBRARIES(toolTestC
  cssimlog
  spliceForTool
)

