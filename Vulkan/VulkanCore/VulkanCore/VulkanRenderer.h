#pragma once

#define VK_NO_PROTOTYPES

#include "VulkanWindow.h"
#include "VulkanContext.h"
#include "VulkanSwapchain.h"

#include <vector>
#include <memory>
#include <cassert>

#include <glm/vec4.hpp>

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

    float getSwapchainAspectRatio()
    {
        return m_vulkanSwapchain->width() / (float)m_vulkanSwapchain->height();
    }

    int getFrameIndex() const
    {
        assert(m_isFrameStarted && "Cannot get frame index when frame not in progress");
        return m_currentFrameIndex;
    }

    void beginFrame(
        VkCommandBuffer& out_CommandBuffer,
        int& out_CurrentImageIndex
    );
    void endFrame(VkCommandBuffer commandBuffer);
    void beginSwapchainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapchainRenderPass(VkCommandBuffer commandBuffer);

    void setClearColor(float r, float g, float b, float a) { m_clearColor = glm::vec4(r, g, b, a); }
    
private:

    VulkanWindow& m_window;
    VulkanContext* m_context;
    std::unique_ptr<VulkanSwapchain> m_vulkanSwapchain;

    std::vector<VkCommandBuffer> m_commandBuffers;

    void recreateSwapchain();

    uint32_t m_currentImageIndex;
    int m_currentFrameIndex;
    bool m_isFrameStarted = false;

    glm::vec4 m_clearColor{ 0, 0, 0, 1 };
};