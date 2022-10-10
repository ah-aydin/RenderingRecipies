#pragma once

#include <fstream>

#include "VulkanContext.h"
#include "VulkanSwapchain.h"

struct VulkanPipelineConfigInfo {
    VulkanPipelineConfigInfo()
        : viewportInfo({}), inputAssemblyInfo({}), rasterizationInfo({}),
        multisampleInfo({}), colorBlendAttachment({}), colorBlendInfo({}),
        depthStencilInfo({}), dynamicStateEnables({}), dynamicStateInfo({})
    {}
    VulkanPipelineConfigInfo(const VulkanPipelineConfigInfo&) = delete;
    VulkanPipelineConfigInfo& operator=(const VulkanPipelineConfigInfo&) = delete;

    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;

    VkShaderStageFlags pushConstantShaderStageFlags;
};

static void defaultPipelineConfigInfo(VulkanPipelineConfigInfo& configInfo)
{
    configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    configInfo.viewportInfo.pScissors = nullptr;
    configInfo.viewportInfo.pViewports = nullptr;
    configInfo.viewportInfo.scissorCount = 1;
    configInfo.viewportInfo.viewportCount = 1;

    configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
    configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
    configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

    configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo.multisampleInfo.minSampleShading = 1.0f;
    configInfo.multisampleInfo.pSampleMask = nullptr;
    configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

    configInfo.colorBlendAttachment = {};
    configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_A_BIT;
    configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
    configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
    configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

    configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
    configInfo.colorBlendInfo.attachmentCount = 1;
    configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
    configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

    configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
    configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
    configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.front = {};  // Optional
    configInfo.depthStencilInfo.back = {};   // Optional

    configInfo.dynamicStateEnables = { VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT };
    configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
    configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
    configInfo.dynamicStateInfo.flags = 0;

    configInfo.pushConstantShaderStageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
}

std::vector<char> readSPIRVFile(const std::string& filepath)
    {
        std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

        if (!file.is_open())
        {
            LOG_ASSERTION(false, "Failed to load file {}", filepath);
        }

        size_t size = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(size);

        file.seekg(0);
        file.read(buffer.data(), size);

        file.close();

        return buffer;
    }

template <typename t_PushConstant, typename t_UniformBuffer, typename t_Vertex>
class VulkanPipeline
{
public:
    VulkanPipeline(
        VulkanContext* context,
        VulkanPipelineConfigInfo& configInfo,
        std::string vertFilePath,
        std::string fragFilePath
    ) : m_context(context)
    {
        LOG_INFO("Creating pipeline");

        LOG_INFO("\tCreating pipeline layout and descriptor set layout");
        {
            // Push constants
            VkPushConstantRange pushConstantRange{};
            //pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            pushConstantRange.stageFlags = configInfo.pushConstantShaderStageFlags;
            pushConstantRange.offset = 0;
            pushConstantRange.size = sizeof(t_PushConstant);

            // Uniform buffers
            VkDescriptorSetLayoutBinding uboLayoutBinding{};
            uboLayoutBinding.binding = 0;
            uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboLayoutBinding.descriptorCount = 1;
            uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            uboLayoutBinding.pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
            descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            descriptorSetLayoutCreateInfo.bindingCount = 1;
            descriptorSetLayoutCreateInfo.pBindings = &uboLayoutBinding;

            VK_CALL(vkCreateDescriptorSetLayout(m_context->getDevice(), &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));

            // Pipeline layout
            VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
            pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
            pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;
            pipelineLayoutCreateInfo.setLayoutCount = 1;
            pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;

            VK_CALL(vkCreatePipelineLayout(m_context->getDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
        }

        LOG_INFO("\tCreating VkPipeline");
        {
            configInfo.pipelineLayout = m_pipelineLayout;
            assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline without a pipeline layout");
            assert(configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline without a render pass");

            auto vertCode = readSPIRVFile(vertFilePath);
            auto fragCode = readSPIRVFile(fragFilePath);

            createShaderModule(vertCode, &m_vertShaderModule);
            createShaderModule(fragCode, &m_fragShaderModule);

            VkPipelineShaderStageCreateInfo stages[2];
            stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
            stages[0].module = m_vertShaderModule;
            stages[0].pName = "main";
            stages[0].flags = 0;
            stages[0].pNext = nullptr;
            stages[0].pSpecializationInfo = nullptr;


            stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            stages[1].module = m_fragShaderModule;
            stages[1].pName = "main";
            
            stages[1].flags = 0;
            stages[1].pNext = nullptr;
            stages[1].pSpecializationInfo = nullptr;

            std::vector<VkVertexInputAttributeDescription> attributeDescriptions = t_Vertex::getAttributeDescriptions();
            std::vector<VkVertexInputBindingDescription> bindingDescriptions = t_Vertex::getBindingDescriptions();

            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
            vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
            vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
            vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = 2;
            pipelineInfo.pStages = stages;
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
            pipelineInfo.pViewportState = &configInfo.viewportInfo;
            pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
            pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
            pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
            pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
            pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

            pipelineInfo.layout = configInfo.pipelineLayout;
            pipelineInfo.renderPass = configInfo.renderPass;
            pipelineInfo.subpass = configInfo.subpass;

            pipelineInfo.basePipelineIndex = -1;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

            VK_CALL(vkCreateGraphicsPipelines(m_context->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline));
        }

        LOG_INFO("\tCreating descriptor pool");
        {
            VkDescriptorPoolSize poolSize{};
            poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSize.descriptorCount = static_cast<uint32_t>(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);

            VkDescriptorPoolCreateInfo poolCreateInfo{};
            poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolCreateInfo.poolSizeCount = 1;
            poolCreateInfo.pPoolSizes = &poolSize;
            poolCreateInfo.maxSets = static_cast<uint32_t>(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);

            VK_CALL(vkCreateDescriptorPool(m_context->getDevice(), &poolCreateInfo, nullptr, &m_descriptorPool));
        }

        LOG_INFO("\tCreating uniform buffers");
        {
            VkDeviceSize bufferSize = sizeof(t_UniformBuffer);

            m_uniformBuffers.resize(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);
            m_uniformBuffersMemory.resize(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);

            for (size_t i = 0; i < VulkanSwapchain::MAX_FRAMES_IN_FLIGHT; ++i)
            {
                m_context->createBuffer(
                    bufferSize,
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    m_uniformBuffers[i],
                    m_uniformBuffersMemory[i]
                );
            }
        }

        LOG_INFO("\tCreating descriptor sets");
        {
            std::vector<VkDescriptorSetLayout> layouts(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT, m_descriptorSetLayout);
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = m_descriptorPool;
            allocInfo.descriptorSetCount = static_cast<uint32_t>(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);
            allocInfo.pSetLayouts = layouts.data();

            m_descriptorSets.resize(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);
            VK_CALL(vkAllocateDescriptorSets(m_context->getDevice(), &allocInfo, m_descriptorSets.data()));

            for (size_t i = 0; i < VulkanSwapchain::MAX_FRAMES_IN_FLIGHT; ++i)
            {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = m_uniformBuffers[i];
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(t_UniformBuffer);

                VkWriteDescriptorSet descriptorWrite{};
                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = m_descriptorSets[i];
                descriptorWrite.dstBinding = 0;
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrite.descriptorCount = 1;
                descriptorWrite.pBufferInfo = &bufferInfo;
                descriptorWrite.pImageInfo = nullptr; // Optional
                descriptorWrite.pTexelBufferView = nullptr; // Optional

                vkUpdateDescriptorSets(m_context->getDevice(), 1, &descriptorWrite, 0, nullptr);
            }
        }
    }

    ~VulkanPipeline()
    {
        LOG_INFO("Destroying pipeline");
        
        LOG_INFO("\tDestroying descriptor pool");
        vkDestroyDescriptorPool(m_context->getDevice(), m_descriptorPool, nullptr);

        LOG_INFO("\tDestorying uniform buffers");
        for (size_t i = 0; i < VulkanSwapchain::MAX_FRAMES_IN_FLIGHT; ++i)
        {
            vkDestroyBuffer(m_context->getDevice(), m_uniformBuffers[i], nullptr);
            vkFreeMemory(m_context->getDevice(), m_uniformBuffersMemory[i], nullptr);
        }


        LOG_INFO("\tDestroying shader modules");
        vkDestroyShaderModule(m_context->getDevice(), m_vertShaderModule, nullptr);
        vkDestroyShaderModule(m_context->getDevice(), m_fragShaderModule, nullptr);

        LOG_INFO("\tDestroying descriptor set layout");
        vkDestroyDescriptorSetLayout(m_context->getDevice(), m_descriptorSetLayout, nullptr);

        LOG_INFO("\tDestroying VkPipeline");
        vkDestroyPipeline(m_context->getDevice(), m_graphicsPipeline, nullptr);

        LOG_INFO("\tDestroying pipeline layout");
        vkDestroyPipelineLayout(m_context->getDevice(), m_pipelineLayout, nullptr);
    }

    void setPushContant(VkCommandBuffer commandBuffer, t_PushConstant& data)
    {
        vkCmdPushConstants(
            commandBuffer,
            m_pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(t_PushConstant),
            &data
        );
    }

    void setUniformBuffer(VkCommandBuffer commandBuffer, t_UniformBuffer& bufferData, int imageIndex)
    {
        VkDeviceMemory bufferMemory = m_uniformBuffersMemory[imageIndex];
        void* data;
        vkMapMemory(m_context->getDevice(), bufferMemory, 0, sizeof(bufferData), 0, &data);
        memcpy(data, &bufferData, sizeof(t_UniformBuffer));
        vkUnmapMemory(m_context->getDevice(), bufferMemory);

        vkCmdBindDescriptorSets(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_pipelineLayout,
            0,
            1,
            &m_descriptorSets[imageIndex],
            0,
            nullptr
        );
    }

    void bind(VkCommandBuffer commandBuffer)
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
    }

private:
    VulkanContext* m_context;
    
    VkPipeline m_graphicsPipeline;
    VkPipelineLayout m_pipelineLayout;
    VkShaderModule m_vertShaderModule;
    VkShaderModule m_fragShaderModule;

    VkDescriptorPool m_descriptorPool;
    VkDescriptorSetLayout m_descriptorSetLayout;
    std::vector<VkDescriptorSet> m_descriptorSets;

    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        createInfo.codeSize = code.size();

        VK_CALL(vkCreateShaderModule(m_context->getDevice(), &createInfo, nullptr, shaderModule));
    }
};