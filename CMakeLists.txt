cmake_minimum_required(VERSION 4.0)
project(brick C)

set(CMAKE_C_STANDARD 11)

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(cglm REQUIRED)
find_package(Freetype REQUIRED)

include_directories(include)

set(brick_lib_src
        src/Window.c
        src/VertexObject.c
        src/Utils.c
        src/Shader.c
        src/GLCheck.c
        src/Camera.c
        src/shape/Rectangle.c
        src/shape/Circle.c
        src/shape/shape.c
)

add_library(brick ${brick_lib_src})
target_link_libraries(brick glad::glad glfw cglm::cglm Freetype::Freetype)

add_executable(brickTest1 main.c)
target_link_libraries(brickTest1 brick)
#target_link_libraries(brick glad::glad glfw3 cglm)
#target_link_libraries(brickTest1 PRIVATE glad::glad glfw cglm::cglm)