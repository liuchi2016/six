# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(D:/androidRedirection/third-party/boost)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include)

add_library( # Sets the name of the library.
             AudioRender
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             src/main/cpp/AudioRender.cpp
             src/main/cpp/Reciever.cpp
             src/main/cpp/UdpSender.cpp
             src/main/cpp/udpAudioUnPacker.cpp)

add_library(avcodec
            STATIC
            IMPORTED)
set_target_properties(avcodec
                      PROPERTIES IMPORTED_LOCATION
                      ${CMAKE_SOURCE_DIR}/static_libs/libavcodec.a)

add_library(avfilter
            STATIC
            IMPORTED)
set_target_properties(avfilter
                      PROPERTIES IMPORTED_LOCATION
                      ${CMAKE_SOURCE_DIR}/static_libs/libavfilter.a)

add_library(avformat
            STATIC
            IMPORTED)
set_target_properties(avformat
                      PROPERTIES IMPORTED_LOCATION
                      ${CMAKE_SOURCE_DIR}/static_libs/libavformat.a)

add_library(avutil
            STATIC
            IMPORTED)
set_target_properties(avutil
                      PROPERTIES IMPORTED_LOCATION
                      ${CMAKE_SOURCE_DIR}/static_libs/libavutil.a)
add_library(swresample
            STATIC
            IMPORTED)
set_target_properties(swresample
                      PROPERTIES IMPORTED_LOCATION
                      ${CMAKE_SOURCE_DIR}/static_libs/libswresample.a)


add_library(swscale
            STATIC
            IMPORTED)
set_target_properties(swscale
                      PROPERTIES IMPORTED_LOCATION
                      ${CMAKE_SOURCE_DIR}/static_libs/libswscale.a)

add_library(boost_system
            STATIC
            IMPORTED)
set_target_properties(boost_system
                      PROPERTIES IMPORTED_LOCATION
                      D:/androidRedirection/third-party/boost/lib/libboost_system.a)

add_library(boost_thread
            STATIC
            IMPORTED)
set_target_properties(boost_thread
                      PROPERTIES IMPORTED_LOCATION
                      D:/androidRedirection/third-party/boost/lib/libboost_thread.a)


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       AudioRender
                       log
                       z
                       android
                       # Links the target library to the log library
                       # included in the NDK.
                       avformat
                       avcodec
                       avfilter
                       avutil
                       swresample
                       swscale
                       boost_system
                       boost_thread)