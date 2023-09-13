cmake_minimum_required(VERSION 3.14)

project(ApiServer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core HttpServer Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core HttpServer Network)

add_executable(ApiServer
  main.cpp
)
target_link_libraries(ApiServer PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::HttpServer Qt${QT_VERSION_MAJOR}::Network QCloudMusicApi)

include(GNUInstallDirs)
install(TARGETS ApiServer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ApiServer)
endif()
