#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <GLES3/gl3.h>
// #include <GLES3/gl32.h>

class VertexArray {
public:
    VertexArray();
    VertexArray(unsigned int t);
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;
    void Delete() const;

private:
    unsigned int m_RendererID;

};

