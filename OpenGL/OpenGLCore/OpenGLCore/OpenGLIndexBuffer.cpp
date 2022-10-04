#include "pch.h"
#include "OpenGLIndexBuffer.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<uint32_t>& indices)
    : m_indexCount(static_cast<uint32_t>(indices.size()))
{
    GL_CALL(glCreateBuffers(1, &m_buffer));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW));
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_buffer));
}

void OpenGLIndexBuffer::bind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer));
}

void OpenGLIndexBuffer::unbind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
