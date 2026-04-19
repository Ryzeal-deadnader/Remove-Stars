cmake_minimum_required(VERSION 3.26)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)

# ==================== EasyX 配置 ====================
set(EASYX_PATH "D:/Easyx")

include_directories(D:/Easyx/include)
link_directories(D:/Easyx/lib64)

add_executable(untitled1 main.c)


target_link_libraries(untitled1 EasyX gdi32 winmm imm32)