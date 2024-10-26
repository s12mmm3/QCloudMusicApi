set(TARGET_NAME QCloudMusicApi)

# Provides dependency openssl
option( WITH_APPS "Build applications" OFF )
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
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Gui)

FILE(GLOB HEADERS ./*.h ./util/*.h)
FILE(GLOB SOURCES ./*.cpp ./util/*.cpp)

FILE(GLOB PUBLIC_HEADERS ./*.h)

add_library(${TARGET_NAME}
    ${HEADERS}
    ${SOURCES}
)
target_compile_definitions(${TARGET_NAME} PRIVATE
    QCLOUDMUSICAPI_LIBRARY
    TARGET_NAME="${TARGET_NAME}"
)
target_compile_definitions(${TARGET_NAME} PRIVATE QT_MESSAGELOGCONTEXT)
set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")

target_link_libraries(${TARGET_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Gui
    ${OPENSSL_SSL_LIBRARY}
    ${OPENSSL_CRYPTO_LIBRARY}
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(${TARGET_NAME} PRIVATE BUILD_SHARED_LIBS)
endif()

install(FILES ${PUBLIC_HEADERS} DESTINATION $<CONFIG>/include/${TARGET_NAME})
install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION $<CONFIG>/bin ARCHIVE DESTINATION $<CONFIG>/lib)
install(DIRECTORY  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/ CONFIGURATIONS Debug DESTINATION $<CONFIG>/bin FILES_MATCHING PATTERN *.pdb)
install(DIRECTORY  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/ CONFIGURATIONS Release DESTINATION $<CONFIG>/bin FILES_MATCHING PATTERN *.pdb)
