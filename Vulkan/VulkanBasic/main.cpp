#include <Core/Log/Log.h>
#include <Core/Event/Event.h>

#include <VulkanCore/Core.h>
#include <VulkanCore/VulkanWindow.h>
#include <VulkanCore/VulkanRenderer.h>
#include <VulkanCore/VulkanPipeline.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

bool running = true;

class EventListener
{
public:
	void onEvent(Event& e)
	{
		if (e.isInCategory(EventCategoryWindow))
		{
			if (e.getType() == EventType::WindowClose)
			{
				running = false;
			}
		}
	}
};

struct PerObjectData
{
    glm::mat4 model;
};

struct PerFrameData
{
    glm::mat4 proj;
    glm::mat4 view;
};

struct Vertex
{
    glm::vec3 pos{};
    glm::vec3 color{};

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescriptions;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
    {
        return {
            // { location, binding, format, offset }
            { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos) },
            { 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) },
        };
    }
};

int main()
{
    Log::Init();
	EventListener eventListener;
    LOG_INFO("Vulkan Basic");
    
	WindowProps windowProps{};
    windowProps.title = "Vuklan Basic";
#ifdef _DEBUG
	windowProps.debug = true;
#endif

	VulkanWindow window(windowProps);
	window.setEventCallback(std::bind(&EventListener::onEvent, &eventListener, std::placeholders::_1));
    VulkanRenderer* renderer = new VulkanRenderer(window);

    VulkanPipelineConfigInfo configInfo;
    defaultPipelineConfigInfo(configInfo);
    configInfo.renderPass = renderer->getSwapchainRenderPass();
    VulkanPipeline<PerObjectData, PerFrameData, Vertex>* pipeline = new VulkanPipeline<PerObjectData, PerFrameData, Vertex>(
        window.getContext(),
        configInfo,
        "res/bin/shader.vert.spv",
        "res/bin/shader.frag.spv"
    );

    std::vector<float> vertices = {
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    std::vector<uint32_t> indices = {
        0, 1, 2, 
    };

    VkBuffer vertexBuffer, indexBuffer;
    VkDeviceMemory vertexBufferMemory, indexBufferMemory;
    VkDeviceSize vertexBufferSize = sizeof(float) * vertices.size();
    VkDeviceSize indexBufferSize = sizeof(uint32_t) * indices.size();
    
    window.getContext()->createBuffer(
        vertexBufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        vertexBuffer,
        vertexBufferMemory
    );
    window.getContext()->createBuffer(
        indexBufferSize,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        indexBuffer,
        indexBufferMemory
    );

    void* vData, *iData;
    vkMapMemory(window.getContext()->getDevice(), vertexBufferMemory, 0, vertexBufferSize, 0, &vData);
    memcpy(vData, vertices.data(), static_cast<size_t>(vertexBufferSize));
    vkUnmapMemory(window.getContext()->getDevice(), vertexBufferMemory);
    vkMapMemory(window.getContext()->getDevice(), indexBufferMemory, 0, indexBufferSize, 0, &iData);
    memcpy(iData, indices.data(), static_cast<size_t>(indexBufferSize));
    vkUnmapMemory(window.getContext()->getDevice(), indexBufferMemory);


	while (running)
	{
        VkCommandBuffer commandBuffer;
        int currentImageIndex;
        renderer->beginFrame(commandBuffer, currentImageIndex);
        if (commandBuffer)
        {
            renderer->beginSwapchainRenderPass(commandBuffer);
            {
                pipeline->bind(commandBuffer);

                PerFrameData perFrameData{};
                perFrameData.view = glm::mat4(1.0f);
                perFrameData.proj = glm::mat4(1.0f);
                perFrameData.view = glm::lookAt(glm::vec3(-2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0));
                perFrameData.proj = glm::perspective(glm::radians(45.0f), renderer->getSwapchainAspectRatio(), 0.1f, 100.0f);
                pipeline->setUniformBuffer(commandBuffer, perFrameData, currentImageIndex);

                PerObjectData perObjectData{};
                glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
                model = glm::rotate(model, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
                perObjectData.model = model;
                pipeline->setPushContant(commandBuffer, perObjectData);

                VkBuffer buffers[] = { vertexBuffer };
                VkDeviceSize offsets[] = { 0 };
                vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
                vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

                vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
            }
            renderer->endSwapchainRenderPass(commandBuffer);
        }
        else
        {
            LOG_ASSERTION(false, "Failed to get command buffer from begin frame.");
        }
        renderer->endFrame(commandBuffer);

		window.update();
	}

    window.getContext()->waitDeviceIdle();

    vkDestroyBuffer(window.getContext()->getDevice(), vertexBuffer, nullptr);
    vkDestroyBuffer(window.getContext()->getDevice(), indexBuffer, nullptr);
    vkFreeMemory(window.getContext()->getDevice(), vertexBufferMemory, nullptr);
    vkFreeMemory(window.getContext()->getDevice(), indexBufferMemory, nullptr);

    delete pipeline;
    delete renderer;

    return 0;
}
