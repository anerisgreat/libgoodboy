cmake_minimum_required (VERSION 2.6)
project (libGoodBoy)
set (${PROJECT_NAME}_VERSION_MAJOR 0)
set (${PROJECT_NAME}_VERSION_MINOR 1)

set (CMAKE_CXX_STANDARD 17)

configure_file (
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.hxx.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.hxx"
    )
include_directories("${PROJECT_BINARY_DIR}")

add_subdirectory(dependencies)
add_subdirectory(lib)
