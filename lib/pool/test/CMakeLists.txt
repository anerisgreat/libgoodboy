add_executable(${PROJECT_NAME}.Test
    MainTestAll.cxx
    TestObjectPool.cxx
    )

target_include_directories(${PROJECT_NAME}.Test
    PRIVATE 
    "${CMAKE_SOURCE_DIR}"
    .
    )

target_link_libraries(${PROJECT_NAME}.Test
    PUBLIC
    ${PROJECT_NAME}
    gtest
    )
