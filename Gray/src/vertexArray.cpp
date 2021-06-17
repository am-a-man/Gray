#include"vertexArray.h"
//#include"renderer.h"
#include"GL/glew.h"
#include<iostream>
#include"vertexBufferLayout.h"


vertexArray::vertexArray()
{
	std::cout << "this is being calle dfrom vertexArray.cpp\n";
	GLCall(glGenVertexArrays(1, &m_rendererID));
	//GLCall(glBindVertexArray(vao));
}




vertexArray::~vertexArray()
{
	GLCall(glDeleteVertexArrays(1,&m_rendererID));
}


void vertexArray::bind() const
{
	GLCall(glBindVertexArray(m_rendererID));
}

void vertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}


void vertexArray::addBuffer(const vertexBuffer& vb, const vertexBufferLayout& layout)
{

	vb.bind();
	bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));  // this number is  the default vertex array object given in compatibility profile
											 // whereas this is not an object in core profile
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized , layout.getStride() , (const void*)offset));
		offset += element.count * vertexBufferElement::getSizeOfType(element.type);
	}
}