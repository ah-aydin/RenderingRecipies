#include "pch.h"
#include "OpenGLVertexArray.h"

OpenGLVertexArray::OpenGLVertexArray(std::vector<float>& vertices, VertexBufferLayout& layout, std::vector<uint32_t>& indices)
{
	GL_CALL(glCreateVertexArrays(1, &m_handle));
	GL_CALL(glBindVertexArray(m_handle));

	m_vertexBuffer = std::make_shared<OpenGLVertexBuffer>(vertices, layout);
	m_indexBuffer = std::make_shared<OpenGLIndexBuffer>(indices);

	GL_CALL(glBindVertexArray(0));
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	GL_CALL(glDeleteVertexArrays(1, &m_handle));
}

void OpenGLVertexArray::bind() const
{
	GL_CALL(glBindVertexArray(m_handle));
}

void OpenGLVertexArray::unbind() const
{
	GL_CALL(glBindVertexArray(0));
}

uint32_t OpenGLVertexArray::getVertexCount() const
{
	return m_vertexBuffer->getVertexCount();
}

uint32_t OpenGLVertexArray::getIndexCount() const
{
	return m_indexBuffer->getIndexCount();
}
