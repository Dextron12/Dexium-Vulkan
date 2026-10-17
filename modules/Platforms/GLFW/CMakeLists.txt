
# Find system apckage( Ensure vcpkg root is set on Windows)
find_package(glfw3 REQUIRED)

file(GLOB_RECURSE GLFW_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE GLFW_INC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_library(Dexium.Window.GLFW STATIC
    ${GLFW_SRC}
    ${GLFW_INC}
)

target_include_directories(Dexium.Window.GLFW PUBLIC
    ${CMAKE_SOURCE_DIR}/modules/
)

#Lionk library
target_link_libraries(Dexium.Window.GLFW PUBLIC
    Dexium
    glfw
)

# Define CPP macro
target_compile_definitions(Dexium.Window.GLFW PUBLIC DX_WM_GLFW=1)