#include "pch.h"
#include "OpenGLVertexBuffer.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<float>& vertices, VertexBufferLayout& layout)
    : m_vertexCount(static_cast<uint32_t>(vertices.size()))
{
    GL_CALL(glCreateBuffers(1, &m_handle));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertexCount, vertices.data(), GL_STATIC_DRAW));

    for (int i = 0; i < layout.getCount(); ++i)
    {
        const VertexBufferElement& element = layout.getElement(i);
        GL_CALL(glEnableVertexAttribArray(i));
        GL_CALL(glVertexAttribPointer(i, element.count, GL_FLOAT, GL_FALSE, layout.getSize(), (void*)element.offset));
    }
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_handle));
}

void OpenGLVertexBuffer::bind() const
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
}

void OpenGLVertexBuffer::unbind() const
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
