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
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/ext/skia/")
	message(FATAL_ERROR "The bx submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

#======== CREATE EXECUTABLE FROM SOURCE FILES ========
# Glob all source files and source headers in the directory and its subdirectories
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
)
# Add your executable
add_executable(app
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/tools/viewer/ImGuiLayer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/tools/viewer/ImGuiLayer.h
)

#======== INCLUDE HEADERS ========
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/third_party/externals/imgui
)

#======== Platform Dependent Logic ========
set(PLATLIBS ${PLATLIBS}
    "-framework ApplicationServices"
    "-framework OpenGL"
    "-framework AppKit"
    "-framework Foundation"
    "-framework QuartzCore"
    "-framework Cocoa"
)

#======== Link Libraries ========
target_link_libraries(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/SKIA/libskia.a
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/TOOLS/libimgui.a
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/TOOLS/obj/tools/sk_app/sk_app.CommandSet.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/TOOLS/obj/tools/sk_app/sk_app.Window.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/TOOLS/obj/tools/sk_app/mac/sk_app.main_mac.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/TOOLS/obj/tools/sk_app/mac/sk_app.Window_mac.o
    ${PLATLIBS}
)
