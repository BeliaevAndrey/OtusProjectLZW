#! /bin/bash

rm -r ./bin
rm -r ./CMakeCache.txt
rm -r ./CMakeFiles
rm -r ./cmake_install.cmake
rm -r ./Makefile
rm -r ./src

# clear

cmake .. && cmake --build .
