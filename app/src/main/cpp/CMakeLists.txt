cmake_minimum_required(VERSION 3.4.1)

#头文件
set(include_dir ${CMAKE_SOURCE_DIR}/include)
include_directories(include_dir)

add_library(cylib SHARED Cylib.cpp)

#引用外部的so包
add_library(libjpeg SHARED IMPORTED)
set_target_properties(libjpeg PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/arm64-v8a/libjpeg.so)

#bitmap的包
find_library(graphics jnigraphics)

#目标库链接其他库
target_link_libraries(cylib libjpeg ${graphics})