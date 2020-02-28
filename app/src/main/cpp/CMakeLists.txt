cmake_minimum_required(VERSION 3.4.1)

add_library(cylib SHARED cylib.cpp)

add_library(libjpeg SHARED IMPORTED)
set_target_properties(libjpeg PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libjpeg.so)

target_link_libraries(cylib libjpeg)