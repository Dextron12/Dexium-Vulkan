

# -- Renderer Backends(RM) modules
# -- Vulkan(Default Renderering module)
if (DX_RENDER_SUBSYSTEM_VULKAN)
    add_subdirectory(RenderBackends/Vulkan)
endif()

# -- Window Manager(WM) modules
# -- GLFW(Default WM)
if(DX_PLATFORM_GLFW)
    add_subdirectory(Platforms/GLFW)
endif()



