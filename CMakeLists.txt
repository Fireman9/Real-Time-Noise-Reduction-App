cmake_minimum_required(VERSION 3.0.0)
project(RTNR VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_PREFIX_PATH "C:/Qt/6.5.0/mingw_64/lib/cmake")

# import vcpkg
include_directories("C:/vcpkg/installed/x64-windows/include")
link_directories("C:/vcpkg/installed/x64-windows/lib")

# import tensorflow-c-api
include_directories("C:/Users/Admin/Documents/Projects/tensorflow-c-api/include")
link_directories("C:/Users/Admin/Documents/Projects/tensorflow-c-api/lib")

# import cppflow
include_directories("C:/Program Files (x86)/cppflow/include")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)
find_package(cppflow REQUIRED)

set(HEADERS src/Stream/AudioStream.h src/Stream/AudioStreamException.h
    src/AudioFile/AudioFile.h src/Filters/Kalman.h
    src/Filters/AdaptiveKalman.h src/Filters/NoiseGate.h
    src/Util/Timer.h
    src/GUI/MainWidget.h src/GUI/Logo/Logo.h
    src/GUI/DropDownList/DropDownList.h src/GUI/ToggleButton/ToggleButton.h
    src/GUI/TextLabel/TextLabel.h src/GUI/Icon/Icon.h
    src/GUI/GateSlider/GateSlider.h src/GUI/AudioChart/AudioChart.h)

set(SOURCES src/main.cpp src/Stream/AudioStream.cpp
    src/Stream/AudioStreamException.cpp src/AudioFile/AudioFile.cpp
    src/Util/Timer.cpp src/Filters/NoiseGate.cpp
    src/GUI/MainWidget.cpp src/GUI/Logo/Logo.cpp
    src/GUI/DropDownList/DropDownList.cpp src/GUI/ToggleButton/ToggleButton.cpp
    src/GUI/TextLabel/TextLabel.cpp src/GUI/Icon/Icon.cpp
    src/GUI/GateSlider/GateSlider.cpp src/GUI/AudioChart/AudioChart.cpp)

add_executable(RTNR ${HEADERS} ${SOURCES})

target_link_libraries(RTNR portAudio)
target_link_libraries(RTNR sndfile)
target_link_libraries(RTNR tensorflow)
target_link_libraries(RTNR cppflow::cppflow)
target_link_libraries(RTNR Qt::Core Qt::Gui Qt::Widgets Qt::Charts)

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

    foreach(QT_LIB Core Gui Widgets Charts)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach(QT_LIB)
endif()
