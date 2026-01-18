#include "Renderer.h"
#include "Log.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "raylib.h"
#include <cstddef>

Renderer::Renderer(Camera3D& mainCam, Camera2D& uiCam)
    : mainCamera3D(mainCam)
    , uiCamera(uiCam)
{
    screenSize.x = GetScreenWidth();
    screenSize.y = GetScreenHeight();
    // renderTarget = LoadRenderTexture(screenSize.x, screenSize.y);
    colorRT = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    SetTextureFilter(colorRT.texture, TEXTURE_FILTER_BILINEAR);
    colorRT.texture.format = PIXELFORMAT_UNCOMPRESSED_R16G16B16A16; // 16*4 bpp (4 channels - half float)
    outlineRT = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    SetTextureFilter(outlineRT.texture, TEXTURE_FILTER_BILINEAR);

    copyShader = *ResourceManager::GetShader(0, "shaders/copy.fs");
    outlineShader = *ResourceManager::GetShader(0, "shaders/outline.fs");
    tonemapShader = *ResourceManager::GetShader(0, "shaders/tonemap.fs");

    if (doBloom) {
        bloomFilterRT = LoadRenderTexture(screenSize.x, screenSize.y);
        SetTextureFilter(bloomFilterRT.texture, TEXTURE_FILTER_BILINEAR);
        bloomFilterRT.texture.format = PIXELFORMAT_UNCOMPRESSED_R16G16B16;
        bloomResultRT = LoadRenderTexture(screenSize.x, screenSize.y);
        SetTextureFilter(bloomResultRT.texture, TEXTURE_FILTER_BILINEAR);

        for (size_t i = 0; i < bloomPyramidRT.size(); i++) {
            // float scale = 1.f / (i + 1) * 4.f;
            bloomPyramidRT[i] = LoadRenderTexture(screenSize.x / (i + 2), screenSize.y / (i + 2));
            SetTextureFilter(bloomPyramidRT[i].texture, TEXTURE_FILTER_BILINEAR);
        }

        float bloomThreshold = 1.f;
        bloomFilterShader = *ResourceManager::GetShader(0, "shaders/bloomFilter.fs");
        SetShaderValue(bloomFilterShader, GetShaderLocation(bloomFilterShader, "_Threshold"), &bloomThreshold,
            SHADER_UNIFORM_FLOAT);
        bloomBlurShader = *ResourceManager::GetShader(0, "shaders/bloomBlur.fs");
        bloomBlurDirectionId = GetShaderLocation(bloomBlurShader, "_Direction");
    }

    // float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };

    SetShaderValue(copyShader, GetShaderLocation(copyShader, "_ScreenSize"), &screenSize, SHADER_UNIFORM_VEC2);
}

Renderer::~Renderer() { }

void Renderer::DrawSky() const
{
    for (Scene* scene : SceneManager::Get().GetScenes()) {
        if (scene != nullptr && scene->IsActive())
            scene->DrawSky();
        // TODO: Draw sky only for the last scene for which sky is available
    }
}

void Renderer::DrawOpaqueGeometry() const
{
    for (Scene* scene : SceneManager::Get().GetScenes()) {
        if (scene != nullptr && scene->IsActive())
            scene->DrawOpaque();
    }
}
void Renderer::DrawTransparentGeometry() const
{
    for (Scene* scene : SceneManager::Get().GetScenes()) {
        if (scene != nullptr && scene->IsActive())
            scene->DrawTransparent();
    }
}
void Renderer::DrawUI() const
{
    for (Scene* scene : SceneManager::Get().GetScenes()) {
        if (scene != nullptr && scene->IsActive())
            scene->DrawUI();
    }
}
void Renderer::AddShader(std::weak_ptr<Shader> shader) { m_Shaders.push_back(shader); }

void Renderer::Render()
{
    // TODO: Render an outline render texture with only objects that will be outlined
    BeginTextureMode(outlineRT); //====================

    ClearBackground(BLANK);

    BeginMode3D(mainCamera3D);
    DrawOpaqueGeometry();
    EndMode3D();

    EndTextureMode(); //===================================

    BeginTextureMode(colorRT); //=== HDR Color ===
    ClearBackground(BLACK);
    BeginMode3D(mainCamera3D);

    DrawSky();
    DrawOpaqueGeometry();

    BeginBlendMode(BLEND_ALPHA);

    EndMode3D();

    BeginShaderMode(outlineShader);
    DrawTextureRec(outlineRT.texture,
        (Rectangle) { 0, 0, (float)outlineRT.texture.width, (float)-outlineRT.texture.height }, (Vector2) { 0, 0 },
        WHITE);
    EndShaderMode();

    BeginMode3D(mainCamera3D);
    DrawTransparentGeometry();

    EndBlendMode();
    EndMode3D();
    EndTextureMode(); // === HDR Color RT End ===

    if (doBloom) {
        // BLOOM
        DrawTextureToTexture(bloomFilterShader, colorRT, bloomFilterRT, true);
        RenderTexture2D from = bloomFilterRT;
        // Downscale
        for (size_t i = 0; i < bloomPyramidRT.size(); i++) {
            SetShaderValue(bloomBlurShader, bloomBlurDirectionId, &horizontal, SHADER_UNIFORM_VEC2);
            DrawTextureToTexture(bloomBlurShader, from, bloomPyramidRT[i], true);

            SetShaderValue(bloomBlurShader, bloomBlurDirectionId, &vertical, SHADER_UNIFORM_VEC2);
            BeginBlendMode(BLEND_ADDITIVE);
            DrawTextureToTexture(bloomBlurShader, from, bloomPyramidRT[i], false);
            EndBlendMode();

            from = bloomPyramidRT[i];
        }
        // Upscale
        DrawTextureToTexture(copyShader, bloomPyramidRT[bloomPyramidRT.size() - 1], bloomResultRT, false);
    }

    BeginDrawing(); //====================================
    ClearBackground(BLACK);

    DrawTextureToScreen(tonemapShader, colorRT, false);

    if (doBloom) {
        BeginBlendMode(BLEND_ADDITIVE);
        // DrawTextureToScreen(copyShader, bloomPyramidRT[bloomPyramidRT.size() - 1], false);
        DrawTextureToScreen(copyShader, bloomResultRT, false);
        EndBlendMode();
    }

    BeginMode2D(uiCamera); // ui overlay
    DrawUI();
    EndMode2D();

    EndDrawing(); //======================================
}

void Renderer::Cleanup()
{
    // UnloadRenderTexture(renderTarget);
    UnloadRenderTexture(colorRT);
    UnloadRenderTexture(outlineRT);
}

void Renderer::DrawTextureToScreen(Shader shader, RenderTexture2D from, bool clear)
{
    BeginShaderMode(shader);
    if (clear)
        ClearBackground(BLACK);
    DrawTextureRec(from.texture, (Rectangle) { 0, 0, (float)from.texture.width, (float)-from.texture.height },
        (Vector2) { 0, 0 }, WHITE);
    EndShaderMode();
}

void Renderer::DrawTextureToTexture(Shader shader, RenderTexture2D from, RenderTexture2D to, bool clear)
{
    BeginTextureMode(to);

    if (clear)
        ClearBackground(BLACK);

    BeginShaderMode(shader);

    Rectangle src = { 0, 0, (float)from.texture.width, (float)-from.texture.height };

    Rectangle dst = { 0, 0, (float)to.texture.width, (float)to.texture.height };

    DrawTexturePro(from.texture, src, dst, { 0, 0 }, 0.0f, WHITE);

    EndShaderMode();
    EndTextureMode();
}
