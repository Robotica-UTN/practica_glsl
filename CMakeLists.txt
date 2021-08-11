cmake_minimum_required(VERSION 3.16)
include(ExternalProject)
project(glsl_practice LANGUAGES C CXX)

# set(PROGRAM_SOURCES 
#     src/render_loop.cpp)
include_directories(
    libs
    libs/GLFW/glfw-3.3.4/include)
add_compile_options(-Wall -Wextra)

ExternalProject_Add(proyect_glwf
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/GLFW/glfw-3.3.4
    BINARY_DIR ${CMAKE_SOURCE_DIR}/libs/GLFW/glfw-3.3.4/build
    STEP_TARGETS build
    EXCLUDE_FROM_ALL TRUE
)

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "executable.exe")
endif (WIN32)

set(CMAKE_CXX_FLAGS_DEBUG "-g2 -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-O2 -DNDEBUG")
set(CMAKE_CXX_STANDARD 11)
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build options are Debug, Release" FORCE)
endif()
add_subdirectory(libs/glad)

file(GLOB PROGRAM_SOURCES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")




add_executable(executable ${PROGRAM_SOURCES})
add_dependencies(executable proyect_glwf-build)

target_link_libraries(executable
    ${CMAKE_SOURCE_DIR}/libs/GLFW/glfw-3.3.4/build/src/libglfw3.a
    glad
    GL X11 pthread Xrandr Xi dl)
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UCASE)
message(STATUS ">> CXX Compile flags (For ${CMAKE_BUILD_TYPE}):\
    ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE_UCASE}}")