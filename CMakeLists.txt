cmake_minimum_required(VERSION 3.2)
project(Micros_Lab_5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Source/main.c
    Source/serial.c
    Source/serial.h
)
add_executable(Micros_Lab_5 ${SOURCE_FILES})