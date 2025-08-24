#pragma once

#include <GLES3/gl3.h>
#include <GLES3/gl32.h>

class VertexBuffer {
public:
    VertexBuffer();
    VertexBuffer(const void* data, GLuint size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
    void Delete() const;

private:
    GLuint m_RendererID;
};
