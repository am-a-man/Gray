#pragma once


#include<GL/glew.h>


#define ASSERT(x) if (!(x)) __debugbreak(); // the underscores (__) in the beginning show that this is platform specific
// and it will only run on visual studio and not on other platforms like gcc/g++


#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLCheckErrors(#x, __FILE__, __LINE__)) // these two __FILE__ and __LINE__ are not platform specific they are supported by all platforms
                                                  // https://www.tutorialspoint.com/what-are-file-line-and-function-in-cplusplus


void GLClearErrors();
bool GLCheckErrors(const char* function, const char* file, const int line);
