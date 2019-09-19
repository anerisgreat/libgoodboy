project(GoodBoy.Pool)

set(${PROJECT_NAME}_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/include")

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE 
    ${${PROJECT_NAME}_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}"
    )

add_subdirectory(test)
