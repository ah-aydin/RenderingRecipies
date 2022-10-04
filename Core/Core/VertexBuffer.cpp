#include "pch.h"
#include "VertexBuffer.h"

VertexBufferLayout::VertexBufferLayout(std::vector<VertexBufferElement> elements)
	: m_elements{elements}, m_size(0)
{
	for (VertexBufferElement& element : m_elements)
	{
		element.offset = m_size;
		m_size += element.size;
	}
}

const VertexBufferElement& VertexBufferLayout::getElement(size_t index) const
{
	return m_elements.at(index);
}