project(libGoodBoy.Neural)

set(${PROJECT_NAME}_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/include" CACHE TYPE STRING)

add_library(${PROJECT_NAME}
    src/Neuron.cxx
    src/InputNeuron.cxx
    src/ConnectableNeuron.cxx
    )

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${${PROJECT_NAME}_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}"
    )

target_link_libraries(${PROJECT_NAME} libGoodBoy.Pool)
