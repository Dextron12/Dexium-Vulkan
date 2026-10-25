cmake_minimum_required(VERSION 3.10)

# Set project name & Ver
project(Dexium VERSION 0.0.1 LANGUAGES CXX)

# CXX Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------
# Options
# -----------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -- Window backend sub-systems
# -- <GLFW>(Default Dexium Window Manager)
option(DX_PLATFORM_GLFW "Use GLFW as the Platform Manager"
        ON
)

# -- Renderer backend sub-systems
# -- <Vulkan>(Default Dexium rendering backend)Note: OpenGL is no longer supported as part of this project!
option(DX_RENDER_SUBSYSTEM_VULKAN "Use Vulkan as the rendering backend"
        ON
)

# -- Test(Sandbox) Project
option(DX_SANDBOX "Compile the Sandbox project"
        ON
)

# Collect project sources
file(GLOB_RECURSE DEXIUM_SRC CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE DEXIUM_INC CONFIGURE_DEPENDS include/*.hpp)

# -----------------------------------------------
# Dexium-wide Dependencies
# -----------------------------------------------
find_package(glm REQUIRED)
find_package(fmt REQUIRED)

# -----------------------------------------------
# Dexium
# -----------------------------------------------

# Create library from globbed sources
add_library(Dexium STATIC ${DEXIUM_SRC})

# Make Library headers visible to IDE's
target_include_directories(Dexium PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link required libraries
target_link_libraries(Dexium PUBLIC
    glm::glm
    fmt::fmt
)

# -----------------------------------------------
# Modules
# -----------------------------------------------
add_subdirectory(modules)

# Connect selected modules to Dexium

if (DX_PLATFORM_GLFW)
    target_link_libraries(Dexium PUBLIC Dexium.Window.GLFW)
    message(STATUS "GLFW WM compiled into Dexium")
endif()

if (DX_RENDER_SUBSYSTEM_VULKAN)
    target_link_libraries(Dexium PUBLIC Dexium.RM.Vulkan)
    message(STATUS "Vulkan RM compiled into Dexium")
endif()

if(DX_SANDBOX)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TestProject/)
    message(STATUS "Building Dexium-Sandbox")
endif()