project(GoodBoy.Neural)

set(${PROJECT_NAME}_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/include")

add_library(${PROJECT_NAME}
    src/Neuron.cxx
    src/InputNeuron.cxx
    src/ConnectableNeuron.cxx
    src/GoodBoyNet.cxx
    src/NeuralGlobals.cxx
    )

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
    ${${PROJECT_NAME}_INCLUDE_DIR}
    PRIVATE
    "${CMAKE_BINARY_DIR}"
    )

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
    GoodBoy.Pool
    nlohmann_json::nlohmann_json
    )

add_subdirectory(test)
