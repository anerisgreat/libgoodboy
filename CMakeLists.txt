cmake_minimum_required (VERSION 2.6)
project (libGoodBoy)
set (${CMAKE_PROJECT_NAME}_VERSION_MAJOR 0)
set (${CMAKE_PROJECT_NAME}_VERSION_MINOR 0)

configure_file (
    "${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}Config.hxx.in"
    "${PROJECT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}Config.hxx"
    )
include_directories("${PROJECT_BINARY_DIR}")

add_subdirectory(lib)
