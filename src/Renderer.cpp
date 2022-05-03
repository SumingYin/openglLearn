#include "Renderer.h"
#include <iostream>
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") " <<
            " " << function << "in file " << file << " :" << line << std::endl;
        return false;
    }
    return true;
}