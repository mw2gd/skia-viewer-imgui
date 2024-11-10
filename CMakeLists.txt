# Author  Matt Walters
# Date    Nov 2 2024
#TODOs     * Set sdl for mobile or glfw for desktop

include(FetchContent)
#======== Set Project Information ========
cmake_minimum_required(VERSION 3.11)
project(fullapp LANGUAGES C CXX OBJC)

# Set C++ standard (C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PLATLIBS ${PLATLIBS} "")
set(OBJECTS ${OBJECTS} "")

#======== DOWNLOAD ALL THE SUBMODULES ========
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

#======== VERIFY FILES DOWNLOADED ========
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/ext/skia/.gn")
	message(FATAL_ERROR "The skia submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

#======== CREATE EXECUTABLE FROM SOURCE FILES ========
# Glob all source files and source headers in the directory and its subdirectories
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
)
# Add your executable
add_executable(fullapp ${SOURCES})

#======== INCLUDE HEADERS ========
target_include_directories(fullapp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/
)

#======== Platform Dependent Logic ========
if(APPLE)
    if(IOS)
        include(cmake/ios.cmake)
    else()
        include(cmake/macos.cmake)
    endif()
endif()

#======== Link Libraries And External Object Files ========
target_link_libraries(fullapp PRIVATE
    # Libraries
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/libskia.a
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/libskshaper.a
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/libimgui.a
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/libsvg.a
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/libskunicode_core.a
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/libskunicode_icu.a
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/libwindow.a
    ${PLATLIBS}

    # Object Files
    ${OBJECTS}
)