#pragma once

#include "Core.h"

#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"

class OpenGLVertexArray 
{
public:
	OpenGLVertexArray(const OpenGLVertexArray&) = delete;
	OpenGLVertexArray& operator=(const OpenGLVertexArray&) = delete;

	OpenGLVertexArray(std::vector<float>& vertices, VertexBufferLayout& layout, std::vector<uint32_t>& indices);
	~OpenGLVertexArray();

	void bind() const;
	void unbind() const;

	uint32_t getVertexCount() const;
	uint32_t getIndexCount() const;

private:
	GLuint m_handle;

	std::shared_ptr<OpenGLVertexBuffer> m_vertexBuffer;
	std::shared_ptr<OpenGLIndexBuffer> m_indexBuffer;
};

