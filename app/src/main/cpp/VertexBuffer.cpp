#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
    : m_RendererID(0)
{
}

VertexBuffer::VertexBuffer(const void* data, GLuint size)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {}

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::Delete() const {glDeleteBuffers(1, &m_RendererID);}
