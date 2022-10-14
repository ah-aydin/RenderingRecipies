#include "pch.h"
#include "VulkanVertexArray.h"

VulkanVertexArray::VulkanVertexArray(VulkanContext* context, std::vector<float>& vertices, std::vector<uint32_t>& indices)
    : m_context(context), m_vertexBuffer(VK_NULL_HANDLE), m_indexBuffer(VK_NULL_HANDLE)
{
    // Vertex Buffer
    {
        VkDeviceSize bufferSize = sizeof(float) * vertices.size();
        context->createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            m_vertexBuffer,
            m_vertexBufferMemory
        );

        void* vData;
        vkMapMemory(context->getDevice(), m_vertexBufferMemory, 0, bufferSize, 0, &vData);
        memcpy(vData, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(context->getDevice(), m_vertexBufferMemory);
    }

    // Index Buffer
    {
        VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();
        context->createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            m_indexBuffer,
            m_indexBufferMemory
        );

        void* iData;
        vkMapMemory(context->getDevice(), m_indexBufferMemory, 0, bufferSize, 0, &iData);
        memcpy(iData, indices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(context->getDevice(), m_indexBufferMemory);
    }
}

VulkanVertexArray::~VulkanVertexArray()
{
    vkDestroyBuffer(m_context->getDevice(), m_vertexBuffer, nullptr);
    vkDestroyBuffer(m_context->getDevice(), m_indexBuffer, nullptr);
    vkFreeMemory(m_context->getDevice(), m_vertexBufferMemory, nullptr);
    vkFreeMemory(m_context->getDevice(), m_indexBufferMemory, nullptr);
}

void VulkanVertexArray::bind(VkCommandBuffer commandBuffer) const
{
    VkBuffer buffers[] = { m_vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void VulkanVertexArray::unbind() const
{
}

uint32_t VulkanVertexArray::getVertexCount() const
{
    return uint32_t();
}

uint32_t VulkanVertexArray::getIndexCount() const
{
    return uint32_t();
}
