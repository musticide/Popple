#pragma once
#include "Assets.h"
#include "GLES3/gl3.h"
#include "stb_image.h"

class Texture : public AndroidAsset {
public:
    Texture();
    Texture(const char* filepath);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
    void Delete() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

private:
    unsigned int m_RendererID;
    const char* m_Filepath;
    GLuint LoadTextureFromAssets(const char* filepath);
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_Channels;
};
