#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#else
#define VK_USE_PLATFORM_XCB_KHR
#endif

#include <vulkan/vulkan.h>

#include <optional>
#include <set>

#include "module.h"

sArenaAllocator* gArena = nullptr;

CEXPORT size_t getDesiredArenaSize() {
    return 1024 * 1024; // Assume 1MB for now, this should be enough for some cases, but bigger games will reallocate this anyway
}

CEXPORT void moduleInit(sArenaAllocator* arena) {
    gArena = arena;
    gArena->debugMode = true;
}

CEXPORT const char* getShaderType() {
    return "glslv";
}

constexpr const char* validationLayers[] = {
    "VK_LAYER_KHRONOS_validation",
};

constexpr const char* deviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

#ifdef DEBUG_BUILD
constexpr bool enableValidationLayers = true;
constexpr bool optimizeShaders = false;
#else
constexpr bool enableValidationLayers = false;
constexpr bool optimizeShaders = true;
#endif

typedef void (*WindowCreateSurfaceFunc)(sWindow, VkInstance, const VkAllocationCallbacks*, VkSurfaceKHR*);

struct sVulkanContext {
    sWindow* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    VkImage* swapChainImages;
    uint32_t swapChainImageCount;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    VkImageView* swapChainImageViews;
};

struct QueueFamiliyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* formats;
    uint32_t formatCount;
    VkPresentModeKHR* presentModes;
    uint32_t presentModeCount;
};

sVulkanContext __vk_ctx;

inline const char* vulkanDebugSeverityName(VkDebugUtilsMessageSeverityFlagBitsEXT severity) {
    switch (severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return "VERBOSE";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return "INFO";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return "WARNING";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return "ERROR";
        default: return "UNKNOWN";
    }
}


VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        printf("[Vulkan Important] [%s] %s\n", vulkanDebugSeverityName(messageSeverity), pCallbackData->pMessage);
    } else {
        printf("[Vulkan Debug] [%s] %s\n", vulkanDebugSeverityName(messageSeverity), pCallbackData->pMessage);
    }
    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

QueueFamiliyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamiliyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)gArena->allocateArray<VkQueueFamilyProperties>(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    VkBool32 presentSupport = false;

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, __vk_ctx.surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }
    }

    return indices;
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, __vk_ctx.surface, &details.capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, __vk_ctx.surface, &details.formatCount, nullptr);
    details.formats = (VkSurfaceFormatKHR*)gArena->allocateArray<VkSurfaceFormatKHR>(details.formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, __vk_ctx.surface, &details.formatCount, details.formats);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, __vk_ctx.surface, &details.presentModeCount, nullptr);
    details.presentModes = (VkPresentModeKHR*)gArena->allocateArray<VkPresentModeKHR>(details.presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, __vk_ctx.surface, &details.presentModeCount, details.presentModes);
    return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const VkSurfaceFormatKHR* availableFormats, uint32_t formatCount) {
    for (size_t i = 0; i < formatCount; i++) {
        if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormats[i];
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const VkPresentModeKHR* availablePresentModes, uint32_t presentModeCount) {
    for (size_t i = 0; i < presentModeCount; i++) {
        if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentModes[i];
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }
    int width, height;
    typedef void (*glfwGetFramebufferSizeFunc)(void*, int*, int*);
    glfwGetFramebufferSizeFunc getFramebufferSize = (glfwGetFramebufferSizeFunc)__vk_ctx.window->creator->lib.getSymbol("glfwGetFramebufferSize");
    getFramebufferSize(__vk_ctx.window->internal, &width, &height);
    VkExtent2D actualExtent = { (uint32_t)width, (uint32_t)height };
    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}

inline int rankPhysicalDevice(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    QueueFamiliyIndices indices = findQueueFamilies(device);
    if (!indices.isComplete()) {
        return 0;
    }
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    VkExtensionProperties* extensions = (VkExtensionProperties*)gArena->allocateArray<VkExtensionProperties>(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions);
    std::set<std::string> requiredExtensions(deviceExtensions, deviceExtensions + sizeof(deviceExtensions) / sizeof(deviceExtensions[0]));
    for (size_t i = 0; i < extensionCount; i++) {
        requiredExtensions.erase(extensions[i].extensionName);
    }
    if (!requiredExtensions.empty()) {
        return 0;
    }

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
    if (swapChainSupport.formatCount == 0 || swapChainSupport.presentModeCount == 0) {
        return 0;
    }

    int score = 1;
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    score += deviceProperties.limits.maxImageDimension2D;

    return score;
}

inline int pickVulkanPhysicalDevice(VkPhysicalDevice* devices, int deviceCount) {
    int bestDeviceIndex = -1;
    int bestScore = 0;
    for (int i = 0; i < deviceCount; i++) {
        int score = rankPhysicalDevice(devices[i]);
        if (score > bestScore) {
            bestScore = score;
            bestDeviceIndex = i;
        }
    }

    if (bestDeviceIndex == -1) {
        printf("Failed to find a suitable GPU\n");
        exit(1);
    } else {
        printf("Selected GPU: %d\n", bestDeviceIndex);
    }

    return bestDeviceIndex;
}

CEXPORT void init(sWindow* win) {
    __vk_ctx = {};
    __vk_ctx.window = win;
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = win->origTitle;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "SpectralEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        const char** extensions;
        int extensionCount;
        win->creator->getInstanceExtensions(*win, &extensions, &extensionCount);
#ifdef __APPLE__
        const int addedExtensions = 2;
#else
        const int addedExtensions = 1;
#endif
        char** newExtensions = gArena->allocateArray<char*>(extensionCount + addedExtensions);
        for (int i = 0; i < extensionCount; i++) {
            newExtensions[i] = gArena->allocateArray<char>(strlen(extensions[i]) + 1);
            strcpy(newExtensions[i], extensions[i]);
        }
        newExtensions[extensionCount] = gArena->allocateArray<char>(strlen(VK_EXT_DEBUG_UTILS_EXTENSION_NAME) + 1);
        strcpy(newExtensions[extensionCount], VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#ifdef __APPLE__
        newExtensions[extensionCount + 1] = gArena->allocateArray<char>(strlen(VK_EXT_METAL_SURFACE_EXTENSION_NAME) + 1);
        strcpy(newExtensions[extensionCount + 1], VK_EXT_METAL_SURFACE_EXTENSION_NAME);
#endif
        createInfo.enabledExtensionCount = extensionCount + addedExtensions;
        createInfo.ppEnabledExtensionNames = newExtensions;
        
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        VkLayerProperties* availableLayers = (VkLayerProperties*)gArena->allocateArray<VkLayerProperties>(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);
        bool validationLayerSupport = true;
        for (size_t i = 0; i < sizeof(validationLayers) / sizeof(validationLayers[0]); i++) {
            bool layerFound = false;
            for (size_t j = 0; j < layerCount; j++) {
                if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) {
                printf("Validation layer %s not found\n", validationLayers[i]);
                validationLayerSupport = false;
            }
        }

        if (enableValidationLayers && validationLayerSupport) {
            createInfo.enabledLayerCount = sizeof(validationLayers) / sizeof(validationLayers[0]);
            createInfo.ppEnabledLayerNames = validationLayers;
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
        }

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = debugCallback;
        debugCreateInfo.pUserData = nullptr; // Optional

        if (enableValidationLayers) {
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        } else {
            createInfo.pNext = nullptr;
        }


        VkResult result = vkCreateInstance(&createInfo, nullptr, &__vk_ctx.instance);
        if (result != VK_SUCCESS) {
            printf("Failed to create Vulkan instance: %d\n", result);
            exit(1);
        }

        if (enableValidationLayers) {
            printf("Validation layers enabled\n");
            VkResult debugResult = CreateDebugUtilsMessengerEXT(__vk_ctx.instance, &debugCreateInfo, nullptr, &__vk_ctx.debugMessenger);
            if (debugResult != VK_SUCCESS) {
                printf("Failed to set up debug messenger: %d\n", debugResult);
                exit(1);
            }
        }
    }
    {
        // our window module (which was selected by the dependency resolver) should support this
        // it should have a function called createWindowSurface
        WindowCreateSurfaceFunc createWindowSurface = (WindowCreateSurfaceFunc)win->creator->lib.getSymbol("createWindowSurface");
        if (createWindowSurface == nullptr) {
            printf("Failed to get createWindowSurface function from window module\n");
            exit(1);
        }
        createWindowSurface(*win, __vk_ctx.instance, nullptr, &__vk_ctx.surface);
        if (__vk_ctx.surface == VK_NULL_HANDLE) {
            printf("Failed to create window surface\n");
            exit(1);
        }

    }
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(__vk_ctx.instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            printf("Failed to find GPUs with Vulkan support\n");
            exit(1);
        }
        VkPhysicalDevice* devices = (VkPhysicalDevice*)gArena->allocateArray<VkPhysicalDevice>(deviceCount);
        vkEnumeratePhysicalDevices(__vk_ctx.instance, &deviceCount, devices);
        int deviceIndex = pickVulkanPhysicalDevice(devices, deviceCount);

        __vk_ctx.physicalDevice = devices[deviceIndex];

        vkGetPhysicalDeviceProperties(__vk_ctx.physicalDevice, &__vk_ctx.physicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(__vk_ctx.physicalDevice, &__vk_ctx.physicalDeviceFeatures);

        QueueFamiliyIndices indices = findQueueFamilies(__vk_ctx.physicalDevice);

        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
        size_t numQueues = uniqueQueueFamilies.size();


        VkDeviceQueueCreateInfo* queueCreateInfos = (VkDeviceQueueCreateInfo*)gArena->allocateArray<VkDeviceQueueCreateInfo>(numQueues);
        float queuePriority = 1.0f;
        for (size_t i = 0; i < numQueues; i++) {
            queueCreateInfos[i] = {};
            queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfos[i].queueFamilyIndex = *std::next(uniqueQueueFamilies.begin(), i);
            queueCreateInfos[i].queueCount = 1;
            queueCreateInfos[i].pQueuePriorities = &queuePriority;
        }

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos;
        deviceCreateInfo.queueCreateInfoCount = numQueues;
        deviceCreateInfo.pEnabledFeatures = &__vk_ctx.physicalDeviceFeatures;
        deviceCreateInfo.enabledExtensionCount = sizeof(deviceExtensions) / sizeof(deviceExtensions[0]);
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
        if (enableValidationLayers) {
            deviceCreateInfo.enabledLayerCount = sizeof(validationLayers) / sizeof(validationLayers[0]);
            deviceCreateInfo.ppEnabledLayerNames = validationLayers;
        } else {
            deviceCreateInfo.enabledLayerCount = 0;
            deviceCreateInfo.ppEnabledLayerNames = nullptr;
        }

        VkResult result = vkCreateDevice(__vk_ctx.physicalDevice, &deviceCreateInfo, nullptr, &__vk_ctx.device);
        if (result != VK_SUCCESS) {
            printf("Failed to create logical device: %d\n", result);
            exit(1);
        }

        vkGetDeviceQueue(__vk_ctx.device, indices.graphicsFamily.value(), 0, &__vk_ctx.graphicsQueue);
        vkGetDeviceQueue(__vk_ctx.device, indices.presentFamily.value(), 0, &__vk_ctx.presentQueue);
    }
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(__vk_ctx.physicalDevice);
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats, swapChainSupport.formatCount);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes, swapChainSupport.presentModeCount);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }
        VkSwapchainCreateInfoKHR swapChainCreateInfo{};
        swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapChainCreateInfo.surface = __vk_ctx.surface;
        swapChainCreateInfo.minImageCount = imageCount;
        swapChainCreateInfo.imageFormat = surfaceFormat.format;
        swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapChainCreateInfo.imageExtent = extent;
        swapChainCreateInfo.imageArrayLayers = 1;
        swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        QueueFamiliyIndices indices = findQueueFamilies(__vk_ctx.physicalDevice);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
        if (indices.graphicsFamily != indices.presentFamily) {
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapChainCreateInfo.queueFamilyIndexCount = 2;
            swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapChainCreateInfo.queueFamilyIndexCount = 0; // Optional
            swapChainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
        }
        swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapChainCreateInfo.presentMode = presentMode;
        swapChainCreateInfo.clipped = VK_TRUE;
        swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE; // Optional

        VkResult result = vkCreateSwapchainKHR(__vk_ctx.device, &swapChainCreateInfo, nullptr, &__vk_ctx.swapChain);
        if (result != VK_SUCCESS) {
            printf("Failed to create swap chain: %d\n", result);
            exit(1);
        }

        vkGetSwapchainImagesKHR(__vk_ctx.device, __vk_ctx.swapChain, &__vk_ctx.swapChainImageCount, nullptr);
        __vk_ctx.swapChainImages = (VkImage*)gArena->allocateArray<VkImage>(__vk_ctx.swapChainImageCount);
        vkGetSwapchainImagesKHR(__vk_ctx.device, __vk_ctx.swapChain, &__vk_ctx.swapChainImageCount, __vk_ctx.swapChainImages);

        __vk_ctx.swapChainImageFormat = surfaceFormat.format;
        __vk_ctx.swapChainExtent = extent;
    }
    {
        __vk_ctx.swapChainImageViews = (VkImageView*)gArena->allocateArray<VkImageView>(__vk_ctx.swapChainImageCount);

        for (size_t i = 0; i < __vk_ctx.swapChainImageCount; i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = __vk_ctx.swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = __vk_ctx.swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            VkResult result = vkCreateImageView(__vk_ctx.device, &createInfo, nullptr, &__vk_ctx.swapChainImageViews[i]);
            if (result != VK_SUCCESS) {
                printf("Failed to create image views: %d\n", result);
                exit(1);
            }
        }
    }
    {

    }
}

CEXPORT void destroy() {
    for (size_t i = 0; i < __vk_ctx.swapChainImageCount; i++) {
        vkDestroyImageView(__vk_ctx.device, __vk_ctx.swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(__vk_ctx.device, __vk_ctx.swapChain, nullptr);
    vkDestroyDevice(__vk_ctx.device, nullptr);

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(__vk_ctx.instance, __vk_ctx.debugMessenger, nullptr);
    }
    vkDestroySurfaceKHR(__vk_ctx.instance, __vk_ctx.surface, nullptr);
    vkDestroyInstance(__vk_ctx.instance, nullptr);
}

CEXPORT void setClearColor(float r, float g, float b, float a) {

}

CEXPORT void clear() {

}

CEXPORT sMesh createMesh(sShader vertexShader, void* vertices, size_t vertexSize, sIndex* indices, size_t indexSize) {

}

CEXPORT void drawMesh(sMesh mesh) {

}

struct InternalShader {
    VkShaderModule shdrMod;
    VkPipelineShaderStageCreateInfo stageInfo;
};

CEXPORT sShader createShader(const char* source, sShaderType type, sVertexDefinition* vertDef, size_t sourceLen) {
    
}

CEXPORT void freeShader(sShader shader) {
    InternalShader* internal = (InternalShader*)shader.internal;
    if (internal != nullptr) {
        vkDestroyShaderModule(__vk_ctx.device, internal->shdrMod, nullptr);
    }
}

CEXPORT void useShaderProgram(sShaderProgram shader) {

}

CEXPORT sShaderProgram createShaderProgram(sShader* shaders, size_t count) {

}

CEXPORT void present() {

}

CEXPORT sUniforms createUniforms(sShaderProgram program, sUniformDefinition def) {

}

CEXPORT void setUniforms(sUniforms uniforms, void* data) {

}

CEXPORT sTexture createTexture(sTextureDefinition tex) {

}

CEXPORT void useTexture(sShaderProgram program, sTexture texture, const char* name) {

}

CEXPORT void freeTexture(sTexture texture) {

}

CEXPORT void freeShaderProgram(sShaderProgram program) {

}

CEXPORT void freeMesh(sMesh mesh) {

}

CEXPORT void freeUniforms(sUniforms uniforms) {

}

CEXPORT void setScissor(int x, int y, int width, int height) {

}

CEXPORT void enableScissor() {

}

CEXPORT void disableScissor() {

}