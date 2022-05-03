#pragma once
#include "VertexBuffer.h"
class VertexBufferLayout; // if we use #include "VertexBufferLayout.h" it will cause circle reference
class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	
	~VertexArray();
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};