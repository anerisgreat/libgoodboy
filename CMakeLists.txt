#Set CMAKE version flag
cmake_minimum_required (VERSION 2.6)

#Setting top-level project for directory
project (libGoodBoy)

#Reading version name from version file
file(STRINGS "${PROJECT_SOURCE_DIR}/version" ${PROJECT_NAME}_VERSION_ARR)

list(GET ${PROJECT_NAME}_VERSION_ARR 0 ${PROJECT_NAME}_VERSION_MAJOR)
list(GET ${PROJECT_NAME}_VERSION_ARR 1 ${PROJECT_NAME}_VERSION_MINOR)
list(GET ${PROJECT_NAME}_VERSION_ARR 2 ${PROJECT_NAME}_VERSION_REVISION)
list(GET ${PROJECT_NAME}_VERSION_ARR 3 ${PROJECT_NAME}_VERSION_BUILD)
set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.\
${${PROJECT_NAME}_VERSION_MINOR}.\
${${PROJECT_NAME}_VERSION_REVISION}.\
${${PROJECT_NAME}_VERSION_BUILD}")

#Set C++ standard (optional)
set (CMAKE_CXX_STANDARD 17)

#Configuration file will be renamed to ProjectNameConfig.hxx
configure_file (
    "${PROJECT_SOURCE_DIR}/Config.hxx.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.hxx"
    )

#Include directory
include_directories("${PROJECT_BINARY_DIR}")

#Add subdirectories
add_subdirectory(dependencies)
add_subdirectory(lib)
