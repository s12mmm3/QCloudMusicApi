cmake_minimum_required(VERSION 3.14)

project(QCloudMusicApi LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OPENSSL_ROOT_DIR ./OPENSSL)

find_package(OpenSSL REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_executable(QCloudMusicApi
  main.cpp
  crypto.hpp
  module.h
  module.cpp
  request.hpp
  config.h
)
target_link_libraries(QCloudMusicApi Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network OpenSSL::SSL OpenSSL::Crypto)

include(GNUInstallDirs)
install(TARGETS QCloudMusicApi
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
