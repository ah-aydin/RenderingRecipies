#pragma once

#include "../Core.h"
#include "../VUlkanContext.h"

#include <vector>

class VulkanVertexArray
{
public:
    VulkanVertexArray(const VulkanVertexArray&) = delete;
    VulkanVertexArray& operator=(const VulkanVertexArray) = delete;

    VulkanVertexArray(VulkanContext* context, std::vector<float>& vertices, std::vector<uint32_t>& indices);
    ~VulkanVertexArray();

    void bind(VkCommandBuffer commandBuffer) const;
    void unbind() const;

    uint32_t getVertexCount() const;
    uint32_t getIndexCount() const;

private:
    VkBuffer m_vertexBuffer;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkDeviceMemory m_indexBufferMemory;

    VulkanContext* m_context;
};

