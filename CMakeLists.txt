# 要求最低Cmake版本
cmake_minimum_required(VERSION 3.14)

# 工程名称和版本
project(QCloudMusicApiProject VERSION 1.0)

# C++标准
set(CMAKE_CXX_STANDARD 17)

# 设置动态库和可执行程序的输出路径为 bin 目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 添加子目录
add_subdirectory(QCloudMusicApi)
add_subdirectory(Test)

find_package(QT NAMES Qt6 Qt5)
find_package(Qt${QT_VERSION_MAJOR})
if(${QT_VERSION} VERSION_GREATER 6.4.0)
    add_subdirectory(ApiServer)
endif()

# 设置安装前缀为二进制目录
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

