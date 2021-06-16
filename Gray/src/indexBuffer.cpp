#include"indexBuffer.h"
#include"renderer.h"

indexBuffer::indexBuffer(const unsigned int* data, unsigned int count)
    :m_count{count}
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));


    GLCall(glGenBuffers(1, &m_renderer_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));
}


indexBuffer::~indexBuffer()
{
    GLCall(glDeleteBuffers(1,&m_renderer_id));
}



void indexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id)); 
}



void indexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

