#pragma once


#include "GL/glew.h"
// here __debugbreak used to stop the program,only in windows
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError;\
x;\
ASSERT(GLCheckError(#x,__FILE__,__LINE__))



// clear all errors
void GLClearError();

bool GLCheckError(const char* function, const char* file, int line);
