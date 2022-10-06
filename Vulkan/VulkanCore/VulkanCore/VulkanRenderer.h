#pragma once

#define VK_NO_PROTOTYPES

#include "VulkanWindow.h"
#include "VulkanContext.h"
#include "VulkanSwapchain.h"

#include <vector>
#include <memory>
#include <cassert>

class VulkanRenderer
{
public:
    VulkanRenderer(VulkanWindow& window);
    ~VulkanRenderer();

    VulkanRenderer(const VulkanRenderer&) = delete;
    VulkanRenderer& operator=(const VulkanRenderer) = delete;

    VkRenderPass getSwapchainRenderPass() const { return m_vulkanSwapchain->getRenderPass(); }
    bool isFrameInProgress() const { return m_isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const
    {
        assert(m_isFrameStarted && "Cannot get command buffer when frame not in progress");
        return m_commandBuffers[m_currentFrameIndex];
    }

    VkDeviceMemory getCurrentUniformBufferMemory() const
    {
        assert(m_isFrameStarted && "Cannot get uniform buffer memory when frame is not in progress");
        return m_uniformBuffersMemory[m_currentFrameIndex];
    }

    float getSwapchainAspectRatio()
    {
        return m_vulkanSwapchain->width() / (float)m_vulkanSwapchain->height();
    }

    int getFrameIndex() const
    {
        assert(m_isFrameStarted && "Cannot get frame index when frame not in progress");
        return m_currentFrameIndex;
    }

    const std::vector<VkBuffer>& getUniformBuffers() const { return m_uniformBuffers; }

    void beginFrame(
        VkCommandBuffer& out_CommandBuffer,
        VkDeviceMemory& out_UniformBufferMemory,
        int& out_CurrentImageIndex
    );
    void endFrame(VkCommandBuffer commandBuffer);
    void beginSwapchainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapchainRenderPass(VkCommandBuffer commandBuffer);
    
private:

    VulkanWindow& m_window;
    VulkanContext* m_context;
    std::unique_ptr<VulkanSwapchain> m_vulkanSwapchain;

    std::vector<VkCommandBuffer> m_commandBuffers;

    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;

    void recreateSwapchain();

    uint32_t m_currentImageIndex;
    int m_currentFrameIndex;
    bool m_isFrameStarted = false;
};