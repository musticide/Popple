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

const void* AndroidAsset::GetBufferFromAsset()
{
    if(m_Asset)
        return AAsset_getBuffer(m_Asset);
    else
        LOGE("Null Reference. Failed to get buffer from Asset. %s", m_Filepath);
        return 0;
}

void AndroidAsset::CloseAsset()
{
    if(m_Asset)
        AAsset_close(m_Asset);
    else
        LOGE("Null Reference. Failed to Close Asset. %s", m_Filepath);
}

