cmake_minimum_required(VERSION 3.14)

project(CApi LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(CApi SHARED
  capi.cpp
  capi.h
)

target_link_libraries(CApi PRIVATE Qt${QT_VERSION_MAJOR}::Core QCloudMusicApi)

target_compile_definitions(CApi PRIVATE CAPI_LIBRARY)
