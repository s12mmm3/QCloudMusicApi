cmake_minimum_required(VERSION 3.5)

project(TestTool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Concurrent)

FILE(GLOB HEADERS ./*.h)
FILE(GLOB SOURCES ./*.cpp)
FILE(GLOB QRCS ./*.qrc)
set(PROJECT_SOURCES
    ${HEADERS}
    ${SOURCES}
    ${QRCS}
)

add_library(${PROJECT_NAME} STATIC
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Concurrent QCloudMusicApi)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TESTTOOL_LIBRARY
    PROJECT_NAME="${PROJECT_NAME}"
)
