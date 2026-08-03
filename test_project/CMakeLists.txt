# Dexium::test_project/CMakeLists.txt

#Project sources
set(TEST_SOURCES
    main.cpp
)

add_executable(TestProject
    ${TEST_SOURCES}
)

target_link_libraries(TestProject PRIVATE Dexium)