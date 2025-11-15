cmake_minimum_required(VERSION 4.0)
project(brick C)

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    find_package(cglm REQUIRED)
    find_package(Freetype REQUIRED)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    # I use brew installed cglm and freetype lib on my mac
    # There are little different, I have to used PkgConfig to find their on cmake
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(cglm REQUIRED IMPORTED_TARGET cglm)
    pkg_check_modules(freetype2 REQUIRED IMPORTED_TARGET freetype2)
endif ()

include_directories(include)
include_directories(3rdparty)

set(brick_lib_src
        src/Window.c
        src/VertexObject.c
        src/Utils.c
        src/Shader.c
        src/GLCheck.c
        src/Texture.c
        src/Font.c
        src/Renderer.c
)

add_library(brick ${brick_lib_src})

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(brick glad::glad glfw cglm::cglm Freetype::Freetype)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    target_link_libraries(brick glad::glad glfw PkgConfig::cglm PkgConfig::freetype2)
endif ()

add_executable(BrickBreaker main.c)
target_link_libraries(BrickBreaker brick)

# Just test graphics lib
add_executable(test1 test1.c)
target_link_libraries(test1 brick)

add_executable(test2 test2.c)
target_link_libraries(test2 brick)