cmake_minimum_required(VERSION 3.14)

project(QCloudMusicApiProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# 生成动态库
set(BUILD_SHARED_LIBS ON)

# set default output path
if(NOT ANDROID AND NOT IOS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
    set(CMAKE_HEADER_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/include)
else()

endif()

add_subdirectory(QCloudMusicApi)
add_subdirectory(Test)

find_package(QT NAMES Qt6 Qt5)
find_package(Qt${QT_VERSION_MAJOR})
if(NOT ANDROID AND NOT IOS)
    add_subdirectory(CApi)
    if(${QT_VERSION} VERSION_GREATER 6.4.0)
        add_subdirectory(ApiServer)
    endif()
endif()

# 设置安装前缀为二进制目录
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

