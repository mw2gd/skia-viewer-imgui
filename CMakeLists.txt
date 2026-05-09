# Author  Matt Walters
# Date    Nov 2 2024
#TODOs     * Set sdl for mobile or glfw for desktop

#======== Set Project Information ========
cmake_minimum_required(VERSION 3.11)
project(app)
# Set C++ standard (C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#======== DOWNLOAD ALL THE SUBMODULES ========
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

#======== VERIFY FILES DOWNLOADED ========
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/ext/skia/BUILD.gn")
       message(FATAL_ERROR "The skia submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and 
try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/ext/imgui/imgui.h")
       message(FATAL_ERROR "The imgui submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and 
try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/ext/SDL/CMakeLists.txt")
       message(FATAL_ERROR "The sdl3 submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and 
try again.")
endif()

#======== CREATE EXECUTABLE FROM SOURCE FILES ========
# Glob all source files in the directory and its subdirectories
file(GLOB_RECURSE SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.mm
)
# Add your executable
add_executable(app ${SOURCES})

#======== INCLUDE HEADERS ========
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/SDL/include
    /usr/local/opt/glew/include
)

#======== Platform Dependent Logic ========
set(PLATLIBS ${PLATLIBS}
    "-framework ApplicationServices"
    "-framework OpenGL"
    "-framework AppKit"
    "-framework Foundation"
    "-framework QuartzCore"
    "-framework MetalKit"

    "-framework AVFoundation"
    "-framework CoreMedia"
    "-framework AudioToolbox"
    "-framework Carbon"
    "-framework Cocoa"
    "-framework CoreAudio"
    "-framework CoreFoundation"
    "-framework CoreHaptics"
    "-framework CoreVideo"
    "-framework ForceFeedback"
    "-framework GameController"
    "-framework IOKit"
    "-framework Metal"
    "-framework UniformTypeIdentifiers"
)

#======== Link Libraries ========
# static libraries vary in names across platforms libskia.a, skia.lib, etc.
file(GLOB SKIA ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/static/*skia.lib)
if(NOT SKIA)
    file(GLOB SKIA ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/static/*skia.a)
endif()

file(GLOB SDL3 ${CMAKE_CURRENT_SOURCE_DIR}/ext/SDL/build/*SDL3.a)
if(NOT SDL3)
    file(GLOB SDL3 ${CMAKE_CURRENT_SOURCE_DIR}/ext/SDL/build/*SDL3.lib)
endif()

target_link_libraries(app PRIVATE
    ${PLATLIBS}
    ${SKIA}
    ${SDL3}
)

#======== Add External Source Files =========
target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/backends/imgui_impl_sdl3.cpp
)