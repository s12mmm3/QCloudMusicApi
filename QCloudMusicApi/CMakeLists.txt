cmake_minimum_required(VERSION 3.14)

project(QCloudMusicApi LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_library(QCloudMusicApi SHARED
    ./util/crypto.h
    ./util/config.h
    ./util/index.h
    module.h
    module.cpp
    request.hpp
)

target_link_libraries(QCloudMusicApi Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network OpenSSL::SSL OpenSSL::Crypto)

target_compile_definitions(QCloudMusicApi PRIVATE QCLOUDMUSICAPI_LIBRARY)

set(PUBLIC_HEADERS
    module.h
)
install(FILES ${PUBLIC_HEADERS} DESTINATION ${CMAKE_HEADER_OUTPUT_DIRECTORY})
install(DIRECTORY ${OPENSSL_INCLUDE_DIR}/../bin DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/../)
