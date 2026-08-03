
#include <fmt/format.h>

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct VersionControl {
    uint8_t variant = 0;
    uint8_t major = 0;
    uint8_t minor = 0;
    uint8_t patch = 0;

    VersionControl(uint8_t variant, uint8_t major, uint8_t minor, uint8_t patch) : variant(variant), major(major), minor(minor), patch(patch) {}
    VersionControl(uint8_t major, uint8_t minor, uint8_t patch) : major(major), minor(minor), patch(patch) {}

    uint32_t makeVersion() const {
        return VK_MAKE_VERSION(major, minor, patch);
    }
};


VersionControl ENGINE_VERSION = {
    0, 0, 0, 1
};

class Instance {
    public:
        Instance(const char* projectName, const VersionControl& projVersion, const VersionControl& vkAPIVersion = {0,1,4,0}) {

            // configure appInfo
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = projectName;
            appInfo.applicationVersion = projVersion.makeVersion();

            appInfo.pEngineName = "Dexium";
            appInfo.engineVersion = ENGINE_VERSION.makeVersion();
            appInfo.apiVersion = vkAPIVersion.makeVersion();

            // COnfigure Instance createInfo:
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
            createInfo.enabledExtensionCount = 0;
            createInfo.ppEnabledExtensionNames = nullptr;

            fmt::print("{}\n", (void*)vkCreateInstance);

            // Attempt to create instance
            if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
                fmt::print(stderr, "Failed to create an instance!\n");
                std::exit(-1);
            }
        }

    ~Instance() {
            if (instance != VK_NULL_HANDLE) {
                vkDestroyInstance(instance, nullptr);
            }
        }

    // Prevent copying, as htis is a RAII class that manages the undelrying VkInstance(calls vkDestroyInstance() on dtor)
    Instance(const Instance&) = delete;
    Instance* operator=(const Instance&) = delete;

    VkInstance getInstance() const {
            return instance; //Internally VkInstance is jsut a handle, cheap tor etunr by value!
        }

    const VkApplicationInfo& getApplicationInfo() const {
        return appInfo;
    }

private:
    VkInstance instance = VK_NULL_HANDLE;

    VkApplicationInfo appInfo{};
    VkInstanceCreateInfo createInfo{};
};

class GLFWInitaliser {
public:

    GLFWInitaliser() {
        if (!glfwInit()) {
            fmt::print(stderr, "Failed to initialize GLFW!\n");
        }

        // Set window hint for NO GL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // For now, disable window resizing:
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    ~GLFWInitaliser() {
        glfwTerminate();
    }


    // pREVENT COPIES
    GLFWInitaliser(const GLFWInitaliser&) = delete;
    GLFWInitaliser& operator=(const GLFWInitaliser&) = delete;

    GLFWInitaliser(GLFWInitaliser&&) = delete;
    GLFWInitaliser& operator=(GLFWInitaliser&&) = delete;
};

class Window {
public:
    Window(const char* windowTitle, int windowWidth, int windowHeight);

private:
    GLFWInitaliser glfwInitaliser;
};

int main() {

    Instance instance("Dexium Test Project", {0,0,0,1});




    return 0;
}