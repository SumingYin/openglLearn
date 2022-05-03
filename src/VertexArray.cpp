#include "VertexArray.h"
#include "Renderer.h"
#include "GL/glew.h"


VertexArray::VertexArray()
{
	
	GLCall(glGenVertexArrays(1, &m_RendererID));
	
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1,&m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{	


	// tell the graphics card the layout of buffer,what's in the memory?
		// when buffer bind,we should enable our bind buffer attribute index,here is zero
	// GLCall(glEnableVertexAttribArray(0));
	// glEnableVertexArrayAttrib(buffer, 0); the second way 
	// const void * with can be ignored,but when 0 change 8,it should be add
	// GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0));
	// location = 0 is the index in glVertexAttribPointer(0,...)


	Bind(); // first Vertex Array bind ;
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for(unsigned int i=0;i<elements.size();i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));
		offset += element.count * VertexBufferElement::GetSizeofType(element.type);
	}
	
	
}


void VertexArray::Bind()
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind()
{
	GLCall(glBindVertexArray(0));
}