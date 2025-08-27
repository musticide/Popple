#include "Texture.h"

Texture::Texture() {
}

Texture::Texture(const char* filepath)
:m_Filepath(filepath), m_RendererID(0)
{
    m_RendererID = LoadTextureFromAssets(filepath);
}

Texture::~Texture() {
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() const
{
    glDeleteTextures(1, &m_RendererID);
}
GLuint Texture::LoadTextureFromAssets(const char* filepath)
{
    OpenAsset(filepath);

    const void* fileData = GetBufferFromAsset();

    if(!fileData)
    {
        LOGE("Failed to get asset buffer: %s", filepath);
        CloseAsset();
        return 0;
    }

    m_LocalBuffer = stbi_load_from_memory((const unsigned char*)fileData, (int)GetAssetLength(), &m_Width, &m_Height,
&m_Channels, 0);

    CloseAsset();

    if(!m_LocalBuffer)
    {
        LOGE("Failed to load image: %s, Error %s", filepath, stbi_failure_reason());
        return 0;
    }

    LOGI("Image loaded: %s", filepath);

    GLenum format;
    switch (m_Channels) {
        case 1:
            format = GL_LUMINANCE;
            break;
        case 2:
            format = GL_LUMINANCE_ALPHA;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            LOGE("Unsupported channel count: %d in %s", m_Channels, filepath);
            stbi_image_free(m_LocalBuffer);
            return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(m_LocalBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    LOGI("Created texture with ID: %u", texture);
    return texture;
}
