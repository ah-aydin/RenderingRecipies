#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <Core/Event/Event.h>
#include <Core/Log/Log.h>
#include <Core/Bitmap/Bitmap.h>
#include <OpenGLCore/OpenGLWindow.h>
#include <OpenGLCore/Shaders/OpenGLProgram.h>
#include <OpenGLCore/Buffers/OpenGLVertexArray.h>
#include <OpenGLCore/Buffers/OpenGLVertexBuffer.h>
#include <OpenGLCore/Textures/Cubemap.h>

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/version.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stb_image.h>

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

struct PerFrameData
{
    glm::mat4 model;
    glm::mat4 mvp;
    glm::vec4 cameraPos;
};

int main()
{
    Log::Init();
	EventListener eventListener;
    LOG_INFO("OpenGLCubemap Demo");
	
	WindowProps windowProps{};
    windowProps.title = "OpenGL Cubemap";
#ifdef _DEBUG
	windowProps.debug = true;
#endif

	Window window(windowProps);
	window.setEventCallback(std::bind(&EventListener::onEvent, &eventListener, std::placeholders::_1));

    OpenGLProgram duckProgram("res/shaderDuck.vert", "res/shaderDuck.frag");
    duckProgram.use();
    GLuint perFrameDataBuffer;
    glCreateBuffers(1, &perFrameDataBuffer);
    glNamedBufferStorage(perFrameDataBuffer, sizeof(PerFrameData), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer, 0, sizeof(PerFrameData));

    VertexBufferLayout layout{ {
		{ VertexBufferType::Float3, "aPos" },
        { VertexBufferType::Float3, "aNormal"},
        { VertexBufferType::Float2, "aTexCoord"}
	}};
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    
    const aiScene* scene = aiImportFile("res/scene.gltf", aiProcess_Triangulate);
    if (!scene || !scene->HasMeshes())
    {
        LOG_ASSERTION(false, "Failed to load mesh");
    }
    const aiMesh* mesh = scene->mMeshes[0];
    for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
    {
        const aiVector3D vertex = mesh->mVertices[i];
        const aiVector3D normal = mesh->mNormals[i];
        const aiVector3D texCoords = mesh->mTextureCoords[0][i];
        vertices.insert(vertices.end(), { vertex.x, vertex.z, vertex.y, normal.x, normal.y, normal.z, texCoords.x, texCoords.y });
    }
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    OpenGLVertexArray duckVertexArray(vertices, layout, indices);
    duckVertexArray.bind();
    
    // Texture
    GLuint textureDuck;
    {
        const char* path = "res/textures/Duck_baseColor.png";
        int w, h, comp;
        const uint8_t* img = stbi_load(path, &w, &h, &comp, 3);
        if (!img)
        {
            LOG_ASSERTION(false, "failed to open file {0}", path);
        }

        GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &textureDuck));
        GL_CALL(glTextureParameteri(textureDuck, GL_TEXTURE_MAX_LEVEL, 0));
        GL_CALL(glTextureParameteri(textureDuck, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTextureParameteri(textureDuck, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTextureStorage2D(textureDuck, 1, GL_RGB8, w, h));
        GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        GL_CALL(glTextureSubImage2D(textureDuck, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, img));
        GL_CALL(glBindTextures(0, 1, &textureDuck));

        stbi_image_free((void*)img);
    }

    // Cube map
    OpenGLProgram cubemapProgram("res/shaderCubemap.vert", "res/shaderCubemap.frag");
    GLuint textureCubemap;
    {
        const char* path = "res/piazza_bologni_1k.hdr";
        int w, h, comp;
        const float* img = stbi_loadf(path, &w, &h, &comp, 3);
        if (!img)
        {
            LOG_ASSERTION(false, "failed to open file {0}", path);
        }
        Bitmap in(w, h, comp, eBitmapFormat_Float, img);
        Bitmap cubemap = convertEquirectangularMapToCubeMapFaces(in);

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureCubemap);
        glTextureParameteri(textureCubemap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureCubemap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureCubemap, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureCubemap, GL_TEXTURE_BASE_LEVEL, 0);
        glTextureParameteri(textureCubemap, GL_TEXTURE_MAX_LEVEL, 0);
        glTextureParameteri(textureCubemap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureCubemap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage2D(textureCubemap, 1, GL_RGB32F, cubemap.getWidth(), cubemap.getHeight());
        const uint8_t* data = cubemap.m_data.data();

        for (uint32_t i = 0; i != 6; i++)
        {
            glTextureSubImage3D(textureCubemap, 0, 0, 0, i, cubemap.getWidth(), cubemap.getHeight(), 1, GL_RGB, GL_FLOAT, data);
            data += cubemap.getWidth() * cubemap.getHeight() * cubemap.getComp() * Bitmap::getBytesPerComponent(cubemap.getFormat());
        }
        glBindTextures(1, 1, &textureCubemap);
    }

    while (running)
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        duckProgram.use();

        const glm::mat4 p = glm::perspective(45.0f, (float) window.getWidth() / window.getHeight(), 0.1f, 100.0f);
        const glm::mat4 v = glm::lookAt(glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.5f), glm::vec3(0.0f, 1.f, 0.0f));
        const glm::mat4 m = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -1.5f)), glm::radians((float)glfwGetTime() * 34.f), glm::vec3(0.0f, 1.0f, 0.0f));
        PerFrameData perFrameData = { m, p * v * m, glm::vec4(0.0f) };
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PerFrameData), &perFrameData);
        glDrawElements(GL_TRIANGLES, duckVertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);

        cubemapProgram.use();
        const glm::mat4 m2 = glm::scale(glm::mat4(1.f), glm::vec3(2.f));
        perFrameData = { m2, p * m2, glm::vec4(0.0f) };
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PerFrameData), &perFrameData);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        window.update();
    }

    glDeleteBuffers(1, &perFrameDataBuffer);
    glDeleteTextures(1, &textureDuck);
    glDeleteTextures(1, &textureCubemap);

    return 0;
}