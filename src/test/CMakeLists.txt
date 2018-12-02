project(libGoodBoy.Test)

set(${PROJECT_NAME}_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/include" CACHE TYPE STRING)

include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
include_directories("${CMAKE_SOURCE_DIR}")

include_directories(${libGoodBoy.Neural_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
    src/Test.cxx
    )

target_link_libraries(${PROJECT_NAME} libGoodBoy.Neural)
