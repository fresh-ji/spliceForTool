
# sources
FILE(GLOB CS_TYPE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# headers
FILE(GLOB CS_TYPE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

#设置工程名称
SET(CURRENT_PROJ_NAME csdds_msg_type)

SET(CS_TYPE_SRC ${CS_TYPE_SRC} ${CS_TYPE_HEADERS} )

#SET(CS_TYPE_HEADERS ${CS_TYPE_HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/msg_type_export.h)

#包含目录
INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include
  ${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include/sys
  ${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/include/dcps/C++/SACPP
  ${CMAKE_SOURCE_DIR}/src/common
)

#库目录
LINK_DIRECTORIES(
  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/spliceForTool/external/OpenSplice/x64/lib
)

#链接库TARGET_
LINK_LIBRARIES(
   ddskernel.lib
   dcpssacpp.lib
   cssimlog
)

#生成库
ADD_LIBRARY(${CURRENT_PROJ_NAME} SHARED ${CS_TYPE_HEADERS} ${CS_TYPE_SRC})
# SET_TARGET_PROPERTIES(${CURRENT_PROJ_NAME} PROPERTIES FOLDER "dds")
ADD_DEFINITIONS(-DOSPL_BUILDEXAMPLE_LIB)
