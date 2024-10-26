set(TARGET_NAME Test)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Concurrent)

FILE(GLOB HEADERS ./*.h)
FILE(GLOB SOURCES ./*.cpp)
FILE(GLOB UIS ./*.ui)
FILE(GLOB QRCS ./*.qrc)
FILE(GLOB CUSTOMWIDGET ./customwidget/*)
set(PROJECT_SOURCES
    ${HEADERS}
    ${SOURCES}
    ${UIS}
    ${QRCS}
    ${CUSTOMWIDGET}
)

set(WITH_TOOLS OFF CACHE BOOL "Build utility tools" FORCE)
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/libqrencode
)

include_directories (./customwidget)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${TARGET_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${TARGET_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${TARGET_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${TARGET_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(${TARGET_NAME} PRIVATE
    TARGET_NAME="${TARGET_NAME}"
)

target_link_libraries(${TARGET_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Concurrent
    QCloudMusicApi
    qrencode
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${TARGET_NAME})
endif()
set_target_properties(${TARGET_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${TARGET_NAME})
endif()

install(TARGETS ${TARGET_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION $<CONFIG>/bin
    ARCHIVE DESTINATION $<CONFIG>/lib
)
