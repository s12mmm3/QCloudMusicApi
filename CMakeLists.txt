cmake_minimum_required(VERSION 3.14)

project(QCloudMusicApiProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

option(MUSICAPI_TEST "Build Test" ON)
option(MUSICAPI_CAPI "Build CApi" ON)
option(MUSICAPI_SERVER "Build ApiServer" ON)
option(MUSICAPI_SETOUTPUTPATH "Set default output path" ON)

# 生成动态库
set(BUILD_SHARED_LIBS ON)

if(MUSICAPI_SETOUTPUTPATH)
    # set default output path
    if(NOT ANDROID AND NOT IOS)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
        set(CMAKE_HEADER_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/include)

        # 设置安装前缀为二进制目录
        set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
    endif()
endif()

add_subdirectory(QCloudMusicApi)
if(MUSICAPI_TEST)
    add_subdirectory(Test)
endif()

if(NOT ANDROID AND NOT IOS)
    if(MUSICAPI_CAPI)
        add_subdirectory(CApi)
    endif()
    find_package(QT NAMES Qt6 Qt5)
    find_package(Qt${QT_VERSION_MAJOR})
    if(${QT_VERSION} VERSION_GREATER 6.4.0)
        # Qt6.4后才支持QHttpServer库
        if(MUSICAPI_SERVER)
            add_subdirectory(ApiServer)
        endif()
    endif()
endif()

