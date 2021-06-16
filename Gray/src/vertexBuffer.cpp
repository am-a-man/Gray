#include"vertexBuffer.h"
#include"renderer.h"

vertexBuffer::vertexBuffer(const void* data, unsigned int size)
{

    GLCall(glGenBuffers(1, &m_renderer_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}


vertexBuffer::~vertexBuffer()
{
    GLCall(glDeleteBuffers(1,&m_renderer_id));
}



void vertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id)); 
}



void vertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

