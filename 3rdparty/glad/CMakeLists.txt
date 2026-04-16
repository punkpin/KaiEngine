cmake_minimum_required(VERSION 3.10)
project(glad)

# 找到 gl.c 的路径 (根据你 README 里的结构，在 src 下)
add_library(glad STATIC src/gl.c)

# 指定头文件路径，这样主工程链接 glad 后就能自动找到 gl.h
target_include_directories(glad PUBLIC include)