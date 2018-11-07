project(libGoodBoy.Core)

set(${PROJECT_NAME}_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/include" CACHE TYPE STRING)

include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
include_directories("${CMAKE_SOURCE_DIR}")

add_library(${PROJECT_NAME}
    src/core.cxx
    )

