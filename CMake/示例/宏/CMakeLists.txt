cmake_minimum_required(VERSION 3.15)
project(demo)

#引入宏模块
include(cmake.cmake)  
add_sub_dir(Linux DemoLinux)
add_sub_dir(DemoWin)

add_executable(demo main.cpp)