#include <vulkan/vulkan.h>

#include "module.h"

sArenaAllocator* gArena = nullptr;

CEXPORT size_t getDesiredArenaSize() {
    return 1024 * 1024; // Assume 1MB for now, this should be enough for some cases, but bigger games will reallocate this anyway
}

CEXPORT void moduleInit(sArenaAllocator* arena) {
    gArena = arena;
}

CEXPORT const char* getShaderType() {
    return "glsl";
}

struct sVulkanContext {
    VkInstance instance;
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    VkRenderPass renderPass;
    VkPipeline pipeline;
    VkFramebuffer framebuffer;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
};

sVulkanContext __vk_ctx;

CEXPORT void init(sWindow* win) {

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = win->origTitle;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "SpectralEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    

}