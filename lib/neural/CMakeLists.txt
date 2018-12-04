project(libGoodBoy.Neural)

set(${PROJECT_NAME}_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/include" CACHE TYPE STRING)

include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
include_directories("${CMAKE_SOURCE_DIR}")

include_directories(${libGoodBoy.Pool_INCLUDE_DIR})

add_library(${PROJECT_NAME}
    src/Neuron.cxx
    src/InputNeuron.cxx
    )

#target_link_libraries(${PROJECT_NAME} libGoodBoy.Pool)
