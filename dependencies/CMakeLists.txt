option(${CMAKE_PROJECT_NAME}_USE_EXTERNAL_JSON "Use an external JSON library" OFF)

if(${CMAKE_PROJECT_NAME}_USE_EXTERNAL_JSON)
    find_package(nlohmann_json 3.2.0 REQUIRED)
else()
    set(JSON_BuildTests OFF CACHE INTERNAL "") 
    add_subdirectory(json) 
endif()
