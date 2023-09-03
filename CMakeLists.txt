# 要求最低Cmake版本
cmake_minimum_required(VERSION 3.14)

# 工程名称和版本
project(QCloudMusicApiProject VERSION 1.0)

# C++标准
set(CMAKE_CXX_STANDARD 17)

# 添加子目录
add_subdirectory(QCloudMusicApi)
add_subdirectory(Test)
