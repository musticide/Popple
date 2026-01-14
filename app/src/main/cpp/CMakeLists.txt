# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.22.1)

# Set standards for C and C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED_ON)

# Set the project name based on the name given on the gradle.properties
project("${APP_LIB_NAME}")

# Include raylib and raymob as a subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/raylib)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/raymob)

# Fetch all source files for your project (recursively), excluding 'deps' source files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/*.c" "${CMAKE_SOURCE_DIR}/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/deps/.*")

# Add headers directory for android_native_app_glue.c
include_directories(${ANDROID_NDK}/sources/android/native_app_glue/)

# Add android_native_app_glue.c to the source files
list(APPEND SOURCES ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# Create a shared library with game source files
add_library(${APP_LIB_NAME} SHARED ${SOURCES})
list(APPEND SOURCES
    "${CMAKE_SOURCE_DIR}/main.cpp"
    "${CMAKE_SOURCE_DIR}/Game.cpp"
    "${CMAKE_SOURCE_DIR}/Renderer.cpp"
    "${CMAKE_SOURCE_DIR}/input.cpp"
    "${CMAKE_SOURCE_DIR}/spatialGrid.cpp"
    "${CMAKE_SOURCE_DIR}/Entity.cpp"
    "${CMAKE_SOURCE_DIR}/DrawableEntity.cpp"
    "${CMAKE_SOURCE_DIR}/bubbleManager.cpp"
    "${CMAKE_SOURCE_DIR}/Signal.cpp"
    "${CMAKE_SOURCE_DIR}/Button.cpp"
    "${CMAKE_SOURCE_DIR}/Tower.cpp"
    "${CMAKE_SOURCE_DIR}/StaticMesh.cpp"
    "${CMAKE_SOURCE_DIR}/ResourceManager.cpp"
    "${CMAKE_SOURCE_DIR}/Scene.cpp"
    "${CMAKE_SOURCE_DIR}/SceneManager.cpp"
    "${CMAKE_SOURCE_DIR}/HomeScene.cpp"
    "${CMAKE_SOURCE_DIR}/GameplayScene.cpp"
    "${CMAKE_SOURCE_DIR}/GameUI.cpp"
    "${CMAKE_SOURCE_DIR}/ParticleSystem.cpp"
    "${CMAKE_SOURCE_DIR}/TouchFeedback.cpp"
    "${CMAKE_SOURCE_DIR}/GameManager.cpp"
    "${CMAKE_SOURCE_DIR}/Globals.cpp"
    "${CMAKE_SOURCE_DIR}/editor/NetworkReceiver.cpp"
    "${CMAKE_SOURCE_DIR}/editor/EditorScene.cpp"
)

# Define compiler macros for the library
target_compile_definitions(${APP_LIB_NAME} PRIVATE PLATFORM_ANDROID)

# Apply flags depending on the build type
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(${APP_LIB_NAME} PRIVATE _DEBUG DEBUG)
endif()

# Include raylib and raymob header files
target_include_directories(${APP_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/deps/raylib")
target_include_directories(${APP_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/deps/raymob")
target_include_directories(${APP_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/deps")

# Link required libraries to the native application
target_link_libraries(${APP_LIB_NAME} raylib raymoblib)
