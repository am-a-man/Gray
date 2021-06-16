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
