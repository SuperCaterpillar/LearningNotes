cmake_minimum_required(VERSION 3.5)
project(MacroDemo)

#引入宏模块
include(cmake.cmake)  
add_sub_dir(Linux linuxDemo)
add_sub_dir(winDemo win)

add_executable(MacroDemo main.cpp)