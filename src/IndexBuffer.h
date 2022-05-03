#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererId;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int * data, unsigned int count); // notion : size is diferent from count
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
};
