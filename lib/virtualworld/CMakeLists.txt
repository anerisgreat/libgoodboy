project(GoodBoy.VirtualWorld)

set(${PROJECT_NAME}_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/include" CACHE TYPE STRING)

add_executable(${PROJECT_NAME}
    src/main.cxx
    )

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${${PROJECT_NAME}_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}"
    )

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
    GoodBoy.Pool
    GoodBoy.Neural
    "Irrlicht"

    PRIVATE
    nlohmann_json::nlohmann_json
    )
