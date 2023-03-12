cmake_minimum_required(VERSION 3.0.0)
project(Sandbox VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

include_directories("C:/vcpkg/installed/x64-windows/include")
link_directories("C:/vcpkg/installed/x64-windows/lib")

set(HEADERS src/AudioStream.h src/AudioStreamException.h
    src/Filters/IIR.h src/AudioFile/AudioFile.h src/SpectralGating.h)

set(SOURCES src/main.cpp src/AudioStream.cpp
    src/AudioStreamException.cpp src/Filters/IIR.cpp
    src/AudioFile/AudioFile.cpp src/SpectralGating.cpp)

add_executable(Sandbox ${HEADERS} ${SOURCES})

target_link_libraries(Sandbox portAudio)
target_link_libraries(Sandbox sndfile)
target_link_libraries(Sandbox fftw3 fftw3f fftw3l)
