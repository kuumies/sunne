#---------------------------------------------------------------------
# Sunne definition

cmake_minimum_required(VERSION 3.0.0)
project(sunne)

#---------------------------------------------------------------------
# Setup compiler

if(MSVC)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_DISABLE_EXTENDED_ALIGNED_STORAGE)
endif(MSVC)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#---------------------------------------------------------------------
# Install

set (CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" CACHE STRING "" FORCE)

#---------------------------------------------------------------------
# OpenMP

find_package(openmp REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

#---------------------------------------------------------------------
# GLFW

set(GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE STRING "" FORCE)
add_subdirectory(external/glfw)
include_directories(external/glfw/include)

#---------------------------------------------------------------------
# GLM

set(BUILD_SHARED_LIBS OFF CACHE STRING "" FORCE)
set(BUILD_STATIC_LIBS OFF CACHE STRING "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE STRING "" FORCE)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DGLM_FORCE_CTOR_INIT)
add_definitions(-D_USE_MATH_DEFINES)
add_subdirectory(external/glm)
include_directories(external/glm)

#---------------------------------------------------------------------
# Glad

add_subdirectory(external/glad)
include_directories(external/glad/include)

#---------------------------------------------------------------------
# stb

include_directories(external/stb)

#---------------------------------------------------------------------
# Sources

file(GLOB_RECURSE GLSL_SOURCES
    "src/*.vsh"
    "src/*.fsh"
)

file(GLOB_RECURSE TEXTURE_SOURCES
    "texture/*.jpg"
    "texture/*.png"
)

file(GLOB_RECURSE CPP_SOURCES
    "src/*.h"
    "src/*.cpp"
)

#---------------------------------------------------------------------
# Exe, link, install

add_executable(${PROJECT_NAME}
    ${CPP_SOURCES}
    ${GLSL_SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${CMAKE_CURRENT_BINARY_DIR}/external/glfw/src/glfw3.lib
    ${CMAKE_CURRENT_BINARY_DIR}/external/glad/gladlib.lib
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

install(FILES ${GLSL_SOURCES}    DESTINATION bin/shaders)
install(FILES ${TEXTURE_SOURCES} DESTINATION bin/textures)
