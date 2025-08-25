#pragma once

#include <GLES2/gl2.h>
#include <EGL/egl.h>
// #include "glm/glm.hpp"
// #include "stb_image.h"
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Log.h"
#include "jni.h"

// extern AAssetManager* g_assetManager;// = nullptr;

class AndroidAsset {
public:
    AndroidAsset();
    ~AndroidAsset();
    static bool InitManager(JNIEnv* env, jobject assetManager);

private:
    static AAssetManager* m_AssetManager;
    const char* m_Filepath;

protected:

    AAsset* m_Asset;

    void OpenAsset(const char* filepath);
    void CloseAsset();
    void ReadAsset(void* buffer);
    size_t GetAssetLength();

};

// GLuint LoadTextureFromAssets(const char* filename);
