#pragma once

#include<iostream>
#include<GL/glew.h>
#include"vertexArray.h"
#include"vertexBufferLayout.h"
#include"indexBuffer.h"
#include"shader.h"

#define ASSERT(x) if (!(x)) __debugbreak(); // the underscores (__) in the beginning show that this is platform specific
// and it will only run on visual studio and not on other platforms like gcc/g++


#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLCheckErrors(#x, __FILE__, __LINE__)) // these two __FILE__ and __LINE__ are not platform specific they are supported by all platforms
                                                  // https://www.tutorialspoint.com/what-are-file-line-and-function-in-cplusplus


void GLClearErrors();
bool GLCheckErrors(const char* function, const char* file, const int line);






class renderer
{
    /*
    what we need to draw something:
        1. vertex buffer
        2. vertex attribute
        3. index buffer
        4. vertex array (already contains vertex buffer and vertex attribute)
        5. shader
        6. uniforms

    */
private:
 
public:
    void clear() const;
    void draw(const vertexArray& vbo,const indexBuffer& ibo ,const shader& shade) const; 
};


