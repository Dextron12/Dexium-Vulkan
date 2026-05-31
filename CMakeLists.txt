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
find_package(fmt REQUIRED)

# Options for Dexium
# -- Rendering Backend Options --
option(DX_VK_BACKEND "Enable Vulkan as the engine backend" ON)
option (DX_GL_BACKEND "Enable OpenGL as the engine backend" OFF) # Deprecating OpenGL from the framework!!

# Enable Vulkan validation layer support. Vk layers are great when debugging Vk issues, enabling this diverts issues into a configured ErrorLogger
option(DX_LayerFeatures "Enable Vulkan layers & reporting back to a configured ErrorLogger" ON)

# Enable/Disable Message logging features
option(DX_ErrorLogging "Enable Error Logging support throughout the engine" ON)

# -- Test Application Compilation --
option(DX_TEST_APPLICATION "Build the Dexium Engine test application" ON)


# Msg for end-users: If compiling in debug mode and DX_LayerFeatures are disabled, nudge them otehrwise!
if (NOT DX_LayerFeatures AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "DX_LayerFeatures are disabled but engine compiled in debug mode. Be aware this may affect Vulkan state tracking and error logging.")
endif()

# Create lib with all sources
add_library(Dexium STATIC ${DEXIUM_SRC})

# Link libraries
target_link_libraries(Dexium
    PUBLIC
        glfw
        glm::glm
        fmt::fmt
)

# Include options
if (DX_VK_BACKEND)
    target_link_libraries(Dexium PUBLIC Vulkan::Vulkan)

    target_compile_definitions(Dexium PUBLIC DX_VK_BACKEND=1)

    message(STATUS "Vulkan backend compiled into Dexium")
endif()

if (DX_GL_BACKEND)
    target_compile_definitions(Dexium PUBLIC DX_GL_BACKEND=1)

    message(STATUS "OpenGL backend compiled into Dexium")
endif()

if (DX_LayerFeatures)
    target_compile_definitions(Dexium PUBLIC DX_LayerFeatures=1)
    message(STATUS "Vulkan Layer Features are enabled")
endif()

if (DX_ErrorLogging)
    target_compile_definitions(Dexium PUBLIC DX_ErrorLogging)
    message(STATUS "Message logging and output enabled")
endif()



# OPTIONALLY compile the Test application found in ./Test_Application
if (DX_TEST_APPLICATION)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Test_Application)
    message(STATUS "Compiling Test Application")
endif()


#Make headers visible to IDEs
target_include_directories(Dexium PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
