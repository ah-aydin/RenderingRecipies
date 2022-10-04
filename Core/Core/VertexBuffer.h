#pragma once

#include <cstdint>
#include <string>

#include "Core.h"

enum class VertexBufferType
{
	None = 0,
	Float, Float2, Float3, Float4,
	MatFloat2, MatFloat3, MatFloat4
};


struct VertexBufferElement
{
public:
	VertexBufferType type;
	std::string name;

	// Number of elements
	uint32_t count;

	// Offset in bytes
	uint32_t offset;

	// Size in bytes
	uint32_t size;

	VertexBufferElement(VertexBufferType type, std::string name)
		: type(type), name(name), count(getBufferTypeCount(type)), size(getBufferTypeSize(type)), offset(0)
	{}
	
private:
	uint32_t getBufferTypeCount(const VertexBufferType& type)
	{
		switch (type)
		{
		case VertexBufferType::Float:		return 1;
		case VertexBufferType::Float2:		return 2;
		case VertexBufferType::Float3:		return 3;
		case VertexBufferType::Float4:		return 4;
		case VertexBufferType::MatFloat2:	return 2 * 2;
		case VertexBufferType::MatFloat3:	return 3 * 3;
		case VertexBufferType::MatFloat4:	return 4 * 4;
		}

		LOG_ASSERTION(false, "Given type does not exist");
		return 0;
	}

	uint32_t getBufferTypeSize(const VertexBufferType& type)
	{
		switch (type)
		{
		case VertexBufferType::Float:		return sizeof(float) * getBufferTypeCount(type);
		case VertexBufferType::Float2:		return sizeof(float) * getBufferTypeCount(type);
		case VertexBufferType::Float3:		return sizeof(float) * getBufferTypeCount(type);
		case VertexBufferType::Float4:		return sizeof(float) * getBufferTypeCount(type);
		case VertexBufferType::MatFloat2:	return sizeof(float) * getBufferTypeCount(type);
		case VertexBufferType::MatFloat3:	return sizeof(float) * getBufferTypeCount(type);
		case VertexBufferType::MatFloat4:	return sizeof(float) * getBufferTypeCount(type);
		}

		LOG_ASSERTION(false, "Given type does not exist");
		return 0;
	}
};

class VertexBufferLayout
{
public:
	//VertexBufferLayout(const VertexBufferLayout&) = delete;
	VertexBufferLayout& operator=(const VertexBufferLayout&) = delete;
	
	VertexBufferLayout(std::vector<VertexBufferElement> elements);

	const VertexBufferElement& getElement(size_t index) const;

	// Number of elements
	size_t getCount() const { return m_elements.size(); }

	// Size in bytes
	uint32_t getSize() { return m_size; }

private:
	std::vector<VertexBufferElement> m_elements;
	uint32_t m_size;
};
