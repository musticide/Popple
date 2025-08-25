#include "Assets.h"
// #include "stb_image.h"

// AAssetManager* g_assetManager = nullptr;
AndroidAsset::AndroidAsset() { }

AndroidAsset::~AndroidAsset() { }

AAssetManager* AndroidAsset::m_AssetManager = nullptr;

bool AndroidAsset::InitManager(JNIEnv* env, jobject assetManager)
{
    m_AssetManager = AAssetManager_fromJava(env, assetManager);
    if (!m_AssetManager) {
        LOGE("Failed to Load Asset Manager");
        return false;
    }
    return true;
}

void AndroidAsset::OpenAsset(const char* filepath)
{
    m_Filepath = filepath;
    if (!m_AssetManager) {
        LOGE("Failed to Load Asset: %s", filepath);
        LOGE("Asset manager not initialized");
        return;
    }
    m_Asset = AAssetManager_open(m_AssetManager, filepath, AASSET_MODE_BUFFER);

    if (!m_Asset)
        LOGE("Failed to Load Asset: %s", filepath);
}

size_t AndroidAsset::GetAssetLength()
{
    if (m_Asset) {
        return AAsset_getLength(m_Asset);
    } else {
        LOGE("Null Reference. Failed to retrieve asset length.: %s", m_Filepath);
        return 0;
    }
}

void AndroidAsset::ReadAsset(void* buffer)
{
    if (m_Asset)
        AAsset_read(m_Asset, buffer, GetAssetLength());
    else
        LOGE("Null Reference. Failed to read Asset. %s", m_Filepath);
}

void AndroidAsset::CloseAsset()
{
    if(m_Asset)
        AAsset_close(m_Asset);
    else
        LOGE("Null Reference. Failed to Close Asset. %s", m_Filepath);
}

/* GLuint LoadTextureFromAssets(const char* filename)
{
    if(!g_assetManager)
    {
        LOGE("Asset Manager not Initialized");
        return  0;
    }

    AAsset* asset = AAssetManager_open(g_assetManager, filename, AASSET_MODE_BUFFER);
    if(!asset)
    {
        LOGE("Failed to open asset: %s", filename);
        return 0;
    }

    off_t fileSize = AAsset_getLength(asset);
    const void* fileData = AAsset_getBuffer(asset);

    if(!fileData)
    {
        LOGE("Failed to get asset buffer: %s", filename);
        AAsset_close(asset);
        return 0;
    }

    int width, height, channels;
    unsigned char* imageData = stbi_load_from_memory((const unsigned char*)fileData, (int)fileSize, &width, &height,
&channels, 0);

    AAsset_close(asset);

    if(!imageData)
    {
        LOGE("Failed to load image: %s, Error %s", filename, stbi_failure_reason());
        return 0;
    }

    LOGI("Image loaded: %s", filename);

    GLenum format;
    switch (channels) {
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
            LOGE("Unsupported channel count: %d in %s", channels, filename);
            stbi_image_free(imageData);
            return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

    LOGI("Created texture with ID: %u", texture);
    return texture;
} */
