cmake_minimum_required(VERSION 3.14)

project(QCloudMusicApi LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS ON)
# Provides dependency openssl
#find_package(OpenSSL QUIET)
if(NOT OPENSSL_FOUND)
    add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/openssl-cmake
        ${CMAKE_CURRENT_BINARY_DIR}/openssl-cmake
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
include_directories (${OPENSSL_INCLUDE_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

FILE(GLOB HEADERS ./*.h ./util/*.h)
FILE(GLOB SOURCES ./*.cpp ./util/*.cpp)
add_library(QCloudMusicApi SHARED
    ${HEADERS}
    ${SOURCES}
)

target_link_libraries(QCloudMusicApi Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})

target_compile_definitions(QCloudMusicApi PRIVATE QCLOUDMUSICAPI_LIBRARY)

set(PUBLIC_HEADERS
    module.h
)
install(FILES ${PUBLIC_HEADERS} DESTINATION ${CMAKE_HEADER_OUTPUT_DIRECTORY})
