cmake_minimum_required(VERSION 3.17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(
        lzw_otus
        lzw_program.cpp
        lzw_file_work.cpp
        lzw_functions.cpp
        ${LOCAL_INCLUDE}/lzw_program.h
        ${LOCAL_INCLUDE}/lzw_file_work.h
        ${LOCAL_INCLUDE}/lzw_functions.h
)

target_include_directories(lzw_otus PUBLIC ${LOCAL_INCLUDE})
