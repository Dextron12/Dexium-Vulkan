cmake_minimum_required(VERSION 3.10)

# Set the project name & version
project(Dexium VERSION 0.0.1 LANGUAGES CXX)

# CXX Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect Project Sources
file(GLOB_RECURSE DEXIUM_SRC CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE DEXIUM_HEADERS CONFIGURE_DEPENDS include/*.hpp)

# Find system packages
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan REQUIRED)

# Options for Dexium
option(DX_VK_BACKEND, "Enable Vulkan as the engine backend" ON)
option (DX_GL_BACKEND, "Enable OpenGL as the engine backend" OFF)

# Create exec with all sources
add_executable(Dexium ${DEXIUM_SRC})

# Link libraries
target_link_libraries(Dexium
    PRIVATE
        glfw
        glm::glm
)

# Include options
if (DX_VK_BACKEND)
    target_link_libraries(Dexium PRIVATE Vulkan::Vulkan)

    target_compile_definitions(Dexium PUBLIC DX_VK_BACKEND=1)

    message(STATUS "Vulkan backend compiled into Dexium")
endif()

if (DX_GL_BACKEND)
    target_compile_definitions(Dexium PUBLIC DX_GL_BACKEND=1)

    message(STATUS, "OpenGL backend compiled into Dexium")
endif()

#Make headers visible to IDEs
target_include_directories(Dexium PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
