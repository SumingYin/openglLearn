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


void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
    //GLCall(glUseProgram(shader)); // use already linked program 
    //GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f)); // uniform use in each per draw different from vertex
    // draw call let graphics card to draw ,system call 
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // function use indices to draw ,instead of glDrawArrays
    // glDrawElements will be used often.

}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

}