cmake_minimum_required(VERSION 3.0.0)
project(Sandbox VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_PREFIX_PATH "C:/Qt/6.4.3/mingw_64/lib/cmake")

include_directories("C:/vcpkg/installed/x64-windows/include")
link_directories("C:/vcpkg/installed/x64-windows/lib")

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADERS src/AudioStream.h src/AudioStreamException.h
    src/AudioFile/AudioFile.h src/Filters/Kalman.h
    src/Filters/AdaptiveKalman.h src/Filters/NoiseGate.h
    src/GUI/MainWidget.h src/GUI/Logo/Logo.h
    src/GUI/DropDownList/DropDownList.h src/GUI/ToggleButton/ToggleButton.h)

set(SOURCES src/main.cpp src/AudioStream.cpp
    src/AudioStreamException.cpp src/AudioFile/AudioFile.cpp
    src/GUI/MainWidget.cpp src/GUI/Logo/Logo.cpp
    src/GUI/DropDownList/DropDownList.cpp src/GUI/ToggleButton/ToggleButton.cpp)

add_executable(Sandbox ${HEADERS} ${SOURCES})

target_link_libraries(Sandbox portAudio)
target_link_libraries(Sandbox sndfile)
target_link_libraries(Sandbox Qt::Core Qt::Gui Qt::Widgets)

if(WIN32)
    set(DEBUG_SUFFIX)

    if(MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif()

    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")

    if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")

        if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif()
    endif()

    if(EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif()

    foreach(QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach(QT_LIB)
endif()
