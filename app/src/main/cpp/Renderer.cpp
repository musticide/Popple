#include "Renderer.h"
#include "Log.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "raylib.h"
#include "utils.h"
#include <algorithm>
#include <cstddef>

Renderer::Renderer(Camera3D& mainCam, Camera2D& uiCam)
: mainCamera3D(mainCam)
, uiCamera(uiCam) {
    screenSize.x = GetScreenWidth();
    screenSize.y = GetScreenHeight();
    // renderTarget = LoadRenderTexture(screenSize.x, screenSize.y);
    colorRT = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    SetTextureFilter(colorRT.texture, TEXTURE_FILTER_BILINEAR);
    colorRT.texture.format = PIXELFORMAT_UNCOMPRESSED_R16G16B16A16; // 16*4 bpp (4 channels - half float)
    outlineRT              = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    SetTextureFilter(outlineRT.texture, TEXTURE_FILTER_BILINEAR);

    copyShader       = *ResourceManager::GetShader(0, "shaders/copy.frag");
    outlineShader    = *ResourceManager::GetShader(0, "shaders/outline.frag");
    tonemapShader    = *ResourceManager::GetShader(0, "shaders/tonemap.frag");
    backgroundShader = *ResourceManager::GetShader(0, "shaders/defaultBackground.frag");

    Utils::SetUniformValue(backgroundShader, "_ScreenSize", &screenSize, SHADER_UNIFORM_VEC2);
    backgroundTexture = *ResourceManager::GetTexture("textures/T_GameBG.png");

    if (doBloom) {
        bloomFilterRT = LoadRenderTexture(screenSize.x, screenSize.y);
        SetTextureFilter(bloomFilterRT.texture, TEXTURE_FILTER_BILINEAR);
        bloomFilterRT.texture.format = PIXELFORMAT_UNCOMPRESSED_R16G16B16;

        bloomResultRT = LoadRenderTexture(screenSize.x, screenSize.y);
        SetTextureFilter(bloomResultRT.texture, TEXTURE_FILTER_BILINEAR);
        bloomResultRT.texture.format = PIXELFORMAT_UNCOMPRESSED_R16G16B16;

        for (size_t i = 0; i < bloomPyramidRT.size(); i++) {
            // last two RT should be of same size for ping-pong
            float scale = 1.f / ((std::min(i, bloomPyramidRT.size() - 2) + 1) * 4.f);

            bloomPyramidRT[i] = LoadRenderTexture(screenSize.x * scale, screenSize.y * scale);
            SetTextureFilter(bloomPyramidRT[i].texture, TEXTURE_FILTER_BILINEAR);
            bloomPyramidRT[i].texture.format = PIXELFORMAT_UNCOMPRESSED_R16G16B16;
        }

        bloomFilterShader = *ResourceManager::GetShader(0, "shaders/bloomFilter.frag");
        Utils::SetUniformValue(bloomFilterShader, "_Threshold", &bloomThreshold, SHADER_UNIFORM_FLOAT);
        bloomBlurShader    = *ResourceManager::GetShader(0, "shaders/bloomBlur.frag");
        bloomComposeShader = *ResourceManager::GetShader(0, "shaders/bloomCompose.frag");
        Utils::SetUniformValue(bloomComposeShader, "_ScreenSize", &screenSize, SHADER_UNIFORM_VEC2);
        bloomBlurDirectionId = GetShaderLocation(bloomBlurShader, "_Direction");
    }

    SetShaderValue(copyShader, GetShaderLocation(copyShader, "_ScreenSize"), &screenSize, SHADER_UNIFORM_VEC2);
}

Renderer::~Renderer() {
}

void Renderer::DrawSky() const {
    for (size_t i = 0; i < SceneManager::Get().scenes.size(); i++) {
        Scene* scene = SceneManager::Get().scenes[i].get();
        if (scene != nullptr && scene->IsActive()) scene->DrawSky();
        // TODO: Draw sky only for the last scene for which sky is available
    }
}

void Renderer::DrawOpaqueGeometry() const {
    for (size_t i = 0; i < SceneManager::Get().scenes.size(); i++) {
        Scene* scene = SceneManager::Get().scenes[i].get();
        if (scene != nullptr && scene->IsActive()) scene->DrawOpaque();
    }
}
void Renderer::DrawTransparentGeometry() const {
    for (size_t i = 0; i < SceneManager::Get().scenes.size(); i++) {
        Scene* scene = SceneManager::Get().scenes[i].get();
        if (scene != nullptr && scene->IsActive()) scene->DrawTransparent();
    }
}
void Renderer::DrawUI() const {
    for (size_t i = 0; i < SceneManager::Get().scenes.size(); i++) {
        Scene* scene = SceneManager::Get().scenes[i].get();
        if (scene != nullptr && scene->IsActive()) scene->DrawUI();
    }
}
void Renderer::AddShader(std::weak_ptr<Shader> shader) {
    m_Shaders.push_back(shader);
}

void Renderer::Render() {
    // TODO: Render an outline render texture with only objects that will be outlined
    BeginTextureMode(outlineRT); //====================

    ClearBackground(BLANK);

    BeginMode3D(mainCamera3D);
    DrawOpaqueGeometry();
    EndMode3D();

    EndTextureMode(); //===================================

    BeginTextureMode(colorRT); //=== HDR Color ===
    ClearBackground(BLANK);
    BeginMode3D(mainCamera3D);

    // DrawSky();
    DrawOpaqueGeometry();

    BeginBlendMode(BLEND_ALPHA);

    EndMode3D();

    DrawRTToScreen(outlineShader, outlineRT, false);

    BeginMode3D(mainCamera3D);
    DrawTransparentGeometry();

    EndBlendMode();
    EndMode3D();
    EndTextureMode(); // === HDR Color RT End ===

    if (doBloom) {
        // BLOOM
        DrawRTtoRT(bloomFilterShader, colorRT, bloomFilterRT, true);
        RenderTexture2D from = bloomFilterRT;
        // Downscale
        for (size_t i = 0; i < bloomPyramidRT.size() - 2; i++) {
            DrawRTtoRT(copyShader, from, bloomPyramidRT[i], false);

            from = bloomPyramidRT[i];
        }
        // blur horizontal
        SetShaderValue(bloomBlurShader, bloomBlurDirectionId, &horizontal, SHADER_UNIFORM_VEC2);
        DrawRTtoRT(bloomBlurShader, from, bloomPyramidRT[bloomPyramidRT.size() - 2], true);

        // blur vertical
        SetShaderValue(bloomBlurShader, bloomBlurDirectionId, &vertical, SHADER_UNIFORM_VEC2);
        DrawRTtoRT(
            bloomBlurShader, bloomPyramidRT[bloomPyramidRT.size() - 2], bloomPyramidRT[bloomPyramidRT.size() - 1], false);

        // Upscale and compose
        DrawRTtoRT(bloomComposeShader, bloomPyramidRT[bloomPyramidRT.size() - 1], bloomResultRT, true);
        // DrawRTtoRT(copyShader, bloomPyramidRT[bloomPyramidRT.size() - 1], bloomResultRT, true);
    }

    /* FINAL COMPOSITION */
    BeginDrawing(); //====================================
    ClearBackground(BLACK);

    DrawTextureToScreen(backgroundShader, backgroundTexture, false);

    if (doBloom) {
        BeginBlendMode(BLEND_ADDITIVE);
        DrawRTToScreen(copyShader, bloomResultRT, false);
        EndBlendMode();
    }

    BeginBlendMode(BLEND_ALPHA);
    DrawRTToScreen(tonemapShader, colorRT, false);
    EndBlendMode();

    BeginMode2D(uiCamera); // ui overlay
    DrawUI();
    EndMode2D();

    EndDrawing(); //======================================
}

void Renderer::Cleanup() {
    // UnloadRenderTexture(renderTarget);
    UnloadRenderTexture(colorRT);
    UnloadRenderTexture(outlineRT);
}

void Renderer::DrawRTToScreen(Shader shader, RenderTexture2D from, bool clear) {
    BeginShaderMode(shader);
    if (clear) ClearBackground(BLACK);
    DrawTextureRec(
        from.texture, (Rectangle){ 0, 0, (float)from.texture.width, (float)-from.texture.height }, (Vector2){ 0, 0 }, WHITE);
    EndShaderMode();
}


void Renderer::DrawTextureToScreen(Shader shader, Texture2D texture, bool clear) {
    BeginShaderMode(shader);
    if (clear) ClearBackground(BLACK);
    Rectangle src = { 0, 0, (float)texture.width, (float)-texture.height };
    Rectangle dst = { 0, 0, (float)screenSize.x, (float)screenSize.y };

    DrawTexturePro(texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndShaderMode();
}

void Renderer::DrawRTtoRT(Shader shader, RenderTexture2D from, RenderTexture2D to, bool clear) {
    BeginTextureMode(to);

    if (clear) ClearBackground(BLACK);

    BeginShaderMode(shader);

    Rectangle src = { 0, 0, (float)from.texture.width, (float)-from.texture.height };

    Rectangle dst = { 0, 0, (float)to.texture.width, (float)to.texture.height };

    DrawTexturePro(from.texture, src, dst, { 0, 0 }, 0.0f, WHITE);

    EndShaderMode();
    EndTextureMode();
}
