project(libGoodBoy.Test)

set(${PROJECT_NAME}_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/include" CACHE TYPE STRING)

add_executable(${PROJECT_NAME}
    src/Test.cxx
    )

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${${PROJECT_NAME}_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}"
    )

target_link_libraries(${PROJECT_NAME} 
    libGoodBoy.Neural
    libGoodBoy.Pool
    )
