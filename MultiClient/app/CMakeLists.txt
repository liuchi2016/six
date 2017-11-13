# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(D:/six/third-party/boost)

add_library( # Sets the name of the library.
             Core
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             src/main/cpp/Core.cpp
             src/main/cpp/App.cpp
             src/main/cpp/DataLogic.cpp
             src/main/cpp/global.cpp
             src/main/cpp/tcpclient.cpp)

add_library(boost_system
            STATIC
            IMPORTED)
set_target_properties(boost_system
                      PROPERTIES IMPORTED_LOCATION
                      D:/six/third-party/boost/lib/libboost_system.a)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       Core
                       log
                       # Links the target library to the log library
                       # included in the NDK.
                       boost_system)