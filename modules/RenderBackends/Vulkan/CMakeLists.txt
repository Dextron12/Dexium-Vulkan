#! Dexium-Modules<RenderModule-Vulkan>

# Find system packages (Ensure vcpkg root is set on Windows)
find_package(Vulkan REQUIRED)

# Glob files together
file(GLOB_RECURSE DXVK_SRC CONFIGURE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE DXVK_INC CONFIGURE ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_library(Dexium.RM.Vulkan STATIC
        ${DXVK_SRC}
        ${DXVK_INC}
)

target_include_directories(Dexium.RM.Vulkan PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(Dexium.RM.Vulkan PUBLIC
    Dexium
    Vulkan::Vulkan
)

#Define CPP macro
target_compile_definitions(Dexium.RM.Vulkan PUBLIC DX_RM_VULKAN=1)
    