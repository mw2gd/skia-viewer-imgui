# Author  Matt Walters
# Date    Nov 2 2024
#TODOs     * Set sdl for mobile or glfw for desktop

include(FetchContent)
#======== Set Project Information ========
cmake_minimum_required(VERSION 3.11)
project(app)
# Set C++ standard (C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



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
add_executable(app ${SOURCES})

FetchContent_Declare(
  zlib
  URL https://zlib.net/zlib-1.3.1.tar.gz
)
FetchContent_MakeAvailable(zlib)

#======== INCLUDE HEADERS ========
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/
    /Users/mattw/repos/imgui/App/ext/sdl3/include/
)

#======== Platform Dependent Logic ========
set(PLATLIBS ${PLATLIBS}
"-framework QuartzCore"
"-framework Cocoa"
"-framework Metal"
"-framework CoreFoundation"
"-framework CoreGraphics"
"-framework CoreText"
"-framework CoreServices"
"-framework IOKit"
"-framework Foundation")
#======== Link Directories ========
target_link_directories(app PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/)
target_link_directories(app PRIVATE  /Users/mattw/repos/imgui/App/ext/sdl3/build/)

#======== Link Libraries ========
target_link_libraries(app PRIVATE
    libskia.a
    libpng.a
    libSDL3.dylib
    ${PLATLIBS}
)
target_link_libraries(app PRIVATE zlibstatic)

#======== Link External Source Files ========