#pragma once

#include "DrawableEntity.h"
#include "Singleton.h"
#include "raylib.h"
#include <array>
#include <memory>
#include <vector>

class Renderer : public Singleton<Renderer> {
public:
    Renderer(Camera3D& mainCam, Camera2D& uiCam);
    ~Renderer();

    void AddShader(std::weak_ptr<Shader> shader);
    void Render();
    void Cleanup();

private:
    std::vector<std::weak_ptr<Shader>> m_Shaders;
    Camera3D mainCamera3D;
    Camera2D uiCamera;
    Vector2 screenSize;

    RenderTexture2D colorRT, outlineRT;
    Shader copyShader, outlineShader, tonemapShader;

    bool doBloom = true;
    RenderTexture2D bloomFilterRT, bloomResultRT;
    std::array<RenderTexture2D, 3> bloomPyramidRT;
    Shader bloomBlurShader, bloomFilterShader;
    int bloomBlurDirectionId;
    Vector2 horizontal = {1.f, 0.f}, vertical = {0.f, 1.f};

    void DrawSky() const;
    void DrawOpaqueGeometry() const;
    void DrawTransparentGeometry() const;
    void DrawUI() const;

    void DrawTextureToScreen(Shader shader, RenderTexture2D from, bool clear);
    void DrawTextureToTexture(Shader shader, RenderTexture2D from, RenderTexture2D to, bool clear);
};
