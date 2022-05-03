#include "VertexBuffer.h"
#include "GL/glew.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererId)); // create buffer and bind,bind buffer like photo shop's layer,choose layer to operate
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
    // offer data to this buffer,docs.gl to see detail about this function
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererId));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // unbind use 0
}