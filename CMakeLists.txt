cmake_minimum_required(VERSION 3.20)
project(meteor)

set(CMAKE_CXX_STANDARD 17)

include_directories("glad/include")

add_executable(meteor main.cpp glad/src/gl.c)
target_link_libraries(meteor GL)
target_link_libraries(meteor glfw)
#target_link_libraries(meteor glm)
