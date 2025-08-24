#include "VertexArray.h"
#include <GLES3/gl3.h>
// #include <GLES3/gl32.h>
#include "VertexBufferLayout.h"

VertexArray::VertexArray() 
:m_RendererID(0){
}

VertexArray::VertexArray(unsigned int t) {
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
}

void VertexArray::Bind() const {
 glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
glBindVertexArray(0);
}

void VertexArray::Delete() const {
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for ( unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glVertexAttribPointer(i, element.count, element.type, element.isNormalized, layout.GetStride()
                              , (const void*)offset);
        glEnableVertexAttribArray(i);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }

}
