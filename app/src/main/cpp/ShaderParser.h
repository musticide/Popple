#pragma once
#include "jni.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include "Assets.h"
#include "Log.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

struct ShaderProgramSource {
    char* VertexShaderSource;
    char* FragmentShaderSource;
    size_t vertexShaderLength;
    size_t fragmentShaderLength;

    void Free()
    {
        if (VertexShaderSource) {
            free(VertexShaderSource);
            VertexShaderSource = nullptr;
        }
        if (FragmentShaderSource) {
            free(FragmentShaderSource);
            FragmentShaderSource = nullptr;
        }
    }
};

ShaderProgramSource ParseShader(const char *filepath)
{
    ShaderProgramSource result;
    result.VertexShaderSource = nullptr;
    result.FragmentShaderSource = nullptr;

    AAsset* asset = AAssetManager_open(g_assetManager, filepath, AASSET_MODE_BUFFER);
    if (!asset) {
        LOGE("Failed to Load Shader: %s", filepath);
        return result;
    }

    size_t length = AAsset_getLength(asset);
    char* buffer = (char*)malloc(length + 1);
    AAsset_read(asset, buffer, length);
    buffer[length] = '\0';
    AAsset_close(asset);

    char* vertStart = strstr(buffer, "#shader vertex");
    char* fragStart = strstr(buffer, "#shader fragment");

    if (!vertStart || !fragStart) {
        free(buffer);
        LOGE("Vertex or Fragment shader directive not found in file: %s", filepath);
        return result;
    }

    vertStart = strchr(vertStart, '\n');
    if (vertStart)
        vertStart++;

    fragStart = strchr(fragStart, '\n');
    if (fragStart)
        fragStart++;

    if (!vertStart || !fragStart) {
        free(buffer);
        LOGE("Vertex or Fragment shader not found in file: %s", filepath);
        return result;
    }

    size_t vertLength, fragLength;

    char* vertEnd = strstr(vertStart, "#shader");
    if(vertEnd) {
        vertLength = vertEnd - vertStart;
    }
    else {
        vertLength = strlen(vertStart);
    }

    char* fragEnd = strstr(fragStart, "#shader");
    if (fragEnd) {
        fragLength = fragEnd - fragStart;
    } else {
        fragLength = strlen(fragStart);
    }

    while (vertLength > 0
        && (vertStart[vertLength - 1] == '\n' || vertStart[vertLength - 1] == '\r'
            || vertStart[vertLength - 1] == ' ')) {
        vertLength--;
    }
    while (fragLength > 0
        && (fragStart[fragLength - 1] == '\n' || fragStart[fragLength - 1] == '\r'
            || fragStart[fragLength - 1] == ' ')) {
        fragLength--;
    }

    result.VertexShaderSource = (char*)malloc(vertLength + 1);
    std::strncpy(result.VertexShaderSource, vertStart, vertLength);
    result.VertexShaderSource[vertLength] = '\0';
    result.vertexShaderLength = vertLength;

    result.FragmentShaderSource = (char*)malloc(fragLength +1);
    std::strncpy(result.FragmentShaderSource, fragStart, fragLength);
    result.FragmentShaderSource[fragLength] = '\0';
    result.fragmentShaderLength = fragLength;

    free(buffer);

    return result;
}

