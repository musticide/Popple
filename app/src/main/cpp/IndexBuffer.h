#pragma once

#include <GLES3/gl3.h>
// #include <GLES3/gl32.h>

class IndexBuffer {
public:
    IndexBuffer();
    IndexBuffer(const void* data, GLuint count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;
    void Delete() const;

    inline GLuint GetCount() const {return m_Count;}

private:
    GLuint m_RendererID;
    GLuint m_Count;
};
