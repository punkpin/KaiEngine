cmake_minimum_required(VERSION 3.10)
project(stb_image)

# 编译 stb_image.cpp
add_library(stb_image STATIC stb_image.cpp)

# 包含当前目录
target_include_directories(stb_image PUBLIC .)