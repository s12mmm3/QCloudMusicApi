cmake_minimum_required(VERSION 3.14)

project(QCloudMusicApiProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

option(MUSICAPI_TEST "Build Test" ON)
# 生成动态库
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# 设置第三方仓库
include_directories(3rdparty)
set(CRYPTOPP_BUILD_TESTING OFF)
set(CRYPTOPP_USE_MASTER_BRANCH ON)
add_subdirectory(3rdparty/cryptopp-cmake/)

set(WITH_TOOLS OFF CACHE BOOL "Build utility tools" FORCE)
add_subdirectory(3rdparty/libqrencode)
include_directories(3rdparty/libqrencode)

add_subdirectory(QCloudMusicApi)
include_directories(QCloudMusicApi)
if(MUSICAPI_TEST)
    add_subdirectory(Test)
endif()
