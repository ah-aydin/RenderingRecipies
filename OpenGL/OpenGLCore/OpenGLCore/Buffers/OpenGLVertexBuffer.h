#pragma once

#include <Core/VertexBuffer.h>
#include "../Core.h"

#include <vector>

class OpenGLVertexBuffer
{
public:
	OpenGLVertexBuffer(const OpenGLVertexBuffer&) = delete;
	OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer&) = delete;

	OpenGLVertexBuffer(std::vector<float>& vertices, VertexBufferLayout& layout);
	~OpenGLVertexBuffer();

	void bind() const;
	void unbind() const;

	uint32_t getVertexCount() { return m_vertexCount; }

private:
	GLuint m_handle;
	uint32_t m_vertexCount;
};
