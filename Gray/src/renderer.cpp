#include"renderer.h"
#include<iostream>


void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLCheckErrors(const char* function, const char* file, const int line)
{
    int i = 0;
    if (GLenum error = glGetError())
    {
        std::cout << "error caught: " << error << " at line: " << line << " in function: " << function << " in file: " << file << std::endl;
        return false;
    }
    return true;

}


void renderer::draw(const vertexArray& vao, const indexBuffer& ibo, const shader& shade) const
{
    vao.bind();
    shade.bind();
    ibo.bind();
    GLCall(glDrawElements(GL_TRIANGLES, ibo.getcount() , GL_UNSIGNED_INT, nullptr));
}



void renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
