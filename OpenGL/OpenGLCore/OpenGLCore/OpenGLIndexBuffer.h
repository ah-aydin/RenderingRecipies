#pragma once

#include "Core.h"

class OpenGLIndexBuffer
{
public:
	OpenGLIndexBuffer(const OpenGLIndexBuffer&) = delete;
	OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer&) = delete;

	OpenGLIndexBuffer(std::vector<uint32_t>& indices);
	~OpenGLIndexBuffer();

	void bind() const;
	void unbind() const;

	uint32_t getIndexCount() const { return m_indexCount; }

private:
	GLuint m_buffer;
	uint32_t m_indexCount;
};
