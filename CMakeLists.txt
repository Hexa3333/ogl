cmake_minimum_required(VERSION 3.10)
project(OGL)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp src/*.h)

add_subdirectory(glfw)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} glad/src/glad.c)

target_include_directories(${PROJECT_NAME} PRIVATE glfw/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_include_directories(${PROJECT_NAME} PRIVATE glad/include/)

target_include_directories(${PROJECT_NAME} PRIVATE glm/)
