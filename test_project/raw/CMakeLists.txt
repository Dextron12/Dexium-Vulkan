cmake_minimum_required(VERSION 3.10)

#Project name & version
project(Dexium VERSION 0.0.1 LANGUAGES CXX)

# CXX Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect Project Sources
set(DEXIUM_SRC main.cpp)

# Find system packages
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(fmt REQUIRED)

add_executable(Dexium ${DEXIUM_SRC})

#Make headers visible to IDEs
#target_include_directories(Dexium PUBLIC ${DEXIUM_INC})

# Link libraries
target_link_libraries(Dexium
    PUBLIC
        glfw
        fmt::fmt
        Vulkan::Vulkan
)

