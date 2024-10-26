cmake_minimum_required(VERSION 3.14)

project(QCloudMusicApi VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows/Macos")

option(MUSICAPI_TEST "Build Test" ON)
option(MUSICAPI_SERVER "Build ApiServer" ON)

# 生成动态库
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

link_directories(${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Test)
