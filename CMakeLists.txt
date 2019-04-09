
#最低版本要求
cmake_minimum_required (VERSION 2.8)

#系统类型
set(CMAKE_SYSTEM_NAME Linux)

#使用交叉编译器?
#set(CROSS_COMPILE "arm-linux-gnueabihf-")
set(CROSS_COMPILE "")

set(CROSS "gcc_4_8_4")

#指定编译器
set(CMAKE_C_COMPILER "${CROSS_COMPILE}gcc")
set(CMAKE_CXX_COMPILER "${CROSS_COMPILE}g++")

#项目名称
project (ui_cpp)

#源文件列表存储到 SRC_LIST
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_LIST)

#把列表 SRC_LIST 的源文件编译成 libsrc.a
add_library(src STATIC ${SRC_LIST})

#添加头文件路径 相当于-I
include_directories(
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/lib/iconv/iconv_for_${CROSS}/include
    ${CMAKE_SOURCE_DIR}/lib/jpeg/jpeg_for_${CROSS}/include
    ${CMAKE_SOURCE_DIR}/lib/freeType2/freeType2_for_${CROSS}/include
    ${CMAKE_SOURCE_DIR}/lib/freeType2/freeType2_for_${CROSS}/include/freetype
    ${CMAKE_SOURCE_DIR}/lib/wpa_client/wpa_client_for_${CROSS}/include
)

#添加库路径 相当于-L
link_directories(
    ${CMAKE_SOURCE_DIR}/lib/iconv/iconv_for_${CROSS}/lib
    ${CMAKE_SOURCE_DIR}/lib/jpeg/jpeg_for_${CROSS}/lib
    ${CMAKE_SOURCE_DIR}/lib/freeType2/freeType2_for_${CROSS}/lib
    ${CMAKE_SOURCE_DIR}/lib/wpa_client/wpa_client_for_${CROSS}/lib
)

#其它编译项
add_definitions(-O3 -Wall)

#app对应源文件
add_executable(app ${CMAKE_SOURCE_DIR}/src/main.cpp)

#指定依赖库 相当于-l
target_link_libraries(app pthread iconv m jpeg freetype wpa_client src)

#指定依赖库 src文件夹生成的libsrc.a
target_link_libraries(app src)
