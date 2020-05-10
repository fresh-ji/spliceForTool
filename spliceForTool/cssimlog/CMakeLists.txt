
# sources
FILE(GLOB CS_LOG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# headers
FILE(GLOB CS_LOG_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# 设置自动生成的头文件到src目录
SET(CS_EXPORT_HEADER ${CMAKE_SOURCE_DIR}/spliceForTool/cssimlog/cssimlog_export.h)
# 设置工程名称
SET(CURRENT_PROJ_NAME cssimlog)

SET(CS_LOG_SRC ${CS_LOG_SRC} ${CS_LOG_HEADERS} )

SET(CS_LOG_HEADERS ${CS_LOG_HEADERS} ${CS_EXPORT_HEADER})

#包含目录
INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/spliceForTool/external/spdlog/include				
)

################################################################################################${CS_BROWSER_MOC_SRCS}

#生成库
ADD_LIBRARY(${CURRENT_PROJ_NAME} SHARED ${CS_LOG_HEADERS} ${CS_LOG_SRC})

# SET_TARGET_PROPERTIES(${CURRENT_PROJ_NAME} PROPERTIES FOLDER "common")

#生成头文件
GENERATE_EXPORT_HEADER(cssimlog
EXPORT_FILE_NAME ${CS_EXPORT_HEADER}
)

#链接库
#TARGET_LINK_LIBRARIES(cssimlog  
  #spdlog 
#)
