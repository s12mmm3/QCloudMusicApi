cmake_minimum_required(VERSION 3.14)

project(QCloudMusicApiProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

option(MUSICAPI_TEST "Build Test" ON)
# 生成动态库
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# 设置第三方仓库
include_directories(3rdparty)
# Provides dependency openssl
option( WITH_APPS "Build applications" OFF )
#find_package(OpenSSL QUIET)
if(NOT OPENSSL_FOUND)
    add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/openssl-cmake
        ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/openssl-cmake
        EXCLUDE_FROM_ALL
    )
    set(OPENSSL_SSL_LIBRARY ssl)
    set(OPENSSL_CRYPTO_LIBRARY crypto)
    set(OPENSSL_INCLUDE_DIR "${openssl_BINARY_DIR}/include" "${openssl_BINARY_DIR}")
    set(OPENSSL_FOUND ON)
    set(OPENSSL_BUILD_SHARED_LIBS ON)
    message(STATUS "Build OpenSSL: ${openssl_BINARY_DIR}")
endif()
set (HAVE_OPENSSL 1)

set(WITH_TOOLS OFF CACHE BOOL "Build utility tools" FORCE)
add_subdirectory(3rdparty/libqrencode)
include_directories(3rdparty/libqrencode)

add_subdirectory(QCloudMusicApi)
include_directories(QCloudMusicApi)
if(MUSICAPI_TEST)
    add_subdirectory(Test)
endif()
