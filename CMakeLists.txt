cmake_minimum_required(VERSION 4.2)

project(colinregistry)

enable_testing()
add_subdirectory(tests)

add_library(${PROJECT_NAME} src/registry.c)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
