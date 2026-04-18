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

# Create exec with all sources
add_executable(Dexium ${DEXIUM_SRC})

# Link libraries
target_link_libraries(Dexium
    PRIVATE
        glfw
        glm::glm
        Vulkan::Vulkan
)

#Make headers visible to IDEs
target_include_directories(Dexium PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
