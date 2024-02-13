cmake_minimum_required(VERSION 3.26)
project(GraphicEditorUp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Programming/CLion/Libraries/Qt/Qt5.12.12/5.12.12/mingw73_64/")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(GraphicEditorUp main.cpp
        GraphicAlgorithms/LineDrawer.cpp
        GraphicAlgorithms/BasicAlgorithms.cpp
        GraphicAlgorithms/ColorSpacesInterpreter.cpp
        GraphicAlgorithms/Disiring.cpp
        GraphicAlgorithms/Filtrator.cpp
        GraphicAlgorithms/GammaChanger.cpp
        GraphicAlgorithms/GistagramChanger.cpp
        GraphicAlgorithms/LineDrawer.cpp
        GraphicAlgorithms/Scaler.cpp
        GraphicAlgorithms/UltimateColor.cpp
        GraphicAlgorithms/PngInterpreter.cpp
        BasicAlgorithms/FileManager.cpp
        GraphicAlgorithms/JpegInterpreter.cpp
        Design/MainWindow.cpp
)
target_link_libraries(GraphicEditorUp
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

if(MINGW)
    target_link_libraries(GraphicEditorUp ws2_32)
endif()


set (LIBDEFLATE TRUE)
set (LIBDEFLATE_DIR C:/Programming/CLion/Libraries/libdeflate-1.19-windows-x86_64-bin)
include_directories(C:/Programming/CLion/Libraries/libdeflate-1.19-windows-x86_64-bin)
include_directories(C:/Programming/CLion/Libraries/libdeflate-1.19/)
target_link_libraries(GraphicEditorUp C:/Programming/CLion/Libraries/libdeflate-1.19-windows-x86_64-bin/libdeflate.dll)

set (LIBDEFLATE_DIR C:/Programming/CLion/Libraries/libdeflate-1.19-windows-x86_64-bin)

