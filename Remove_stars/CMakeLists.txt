cmake_minimum_required(VERSION 3.26)
project(Remove-stars)

set(CMAKE_CXX_STANDARD 17)

# 1. 生成可执行文件（你的目标名 = Remove_stars）
add_executable(Remove-stars main.cpp)

# 2. 官方文档要求的链接命令！！！
target_link_libraries(Remove-stars libeasyxw.a)