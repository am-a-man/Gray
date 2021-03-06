#pragma once


#include<GL/glew.h>
#include<vector>
#include "renderer.h"

struct vertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
	
		}
	}
};


class vertexBufferLayout
{
private:
	std::vector<vertexBufferElement> m_elements;
	unsigned int m_stride = 0;
public:
	template<class type>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count )
	{
		m_elements.push_back({GL_FLOAT, count ,GL_FALSE });
		m_stride += vertexBufferElement::getSizeOfType(GL_FLOAT)*count;
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count ,GL_FALSE });
		m_stride += count*vertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);

	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count ,GL_TRUE });
		m_stride += count*vertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<vertexBufferElement> getElements() const { return m_elements; }
	inline unsigned int getStride() const { return m_stride; };

};