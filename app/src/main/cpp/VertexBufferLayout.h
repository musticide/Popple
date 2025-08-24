#pragma once
#include "Log.h"
#include <GLES3/gl3.h>
// #include <GLES3/gl32.h>
#include <vector>

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char isNormalized;

    static unsigned int GetSizeOfType(unsigned int type)
{
        switch (type) {
            case GL_FLOAT : return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        LOGE("Could not get size: Type not found");
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout()
    :m_Stride(0) {}
    ~VertexBufferLayout(){}

    template<typename T>
    void Push(unsigned int count)
    {
        // static_assert(false);
        LOGE("ASSERT");
    }

    template<>
    void Push<float>(unsigned int count)
    {
        m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
    }

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements;}
    inline unsigned int GetStride() const {return m_Stride;}
};
