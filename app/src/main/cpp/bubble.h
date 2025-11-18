#pragma once

#include "Log.h"
#include "raylib.h"
#include "raymath.h"
#include <raymob.h>
#include <vector>

#include "Entity.h"
#include "input.h"

enum BubbleType { DEFAULT_BUBBLE = 0, ELECTRO_BUBBLE, ANEMO_BUBBLE, BUBBLE_TYPES_COUNT };

class Bubble : public Entity {
private:
    bool m_IsActive = true;

    // Physics
    BubbleType m_Type;
    Vector3 m_Velocity;
    constexpr static const float m_Drag = 0.1;
    constexpr static const float m_CenterForce = 0.01;
    // std::vector<Vector3> m_Forces;

    // Rendering
    static Model s_BubbleBaseModel;
    static Shader s_BubbleShader;
    static TextureCubemap s_EnvironmentMap;
    Model m_BubbleBaseModel;

public:
    Bubble();
    ~Bubble();

    Vector3 position;
    float rotation;
    float radius;
    Color color;
    float maxMoveSpeed = 15;
    float moveSpeed = 1.0;

    void Start() override;
    void Update(float dT = 1.0f) override;

    void Draw() const override;

    void AddForce(Vector3 force);

    void Spawn();

    bool IsPointInBubble(Vector3 point) const;

    // Getters
    Vector3 GetVelocity() const { return m_Velocity; }
    BubbleType GetType() const { return m_Type; }

    static void LoadBubbleFiles()
    {
        static bool isModelLoaded = false;
        if (!isModelLoaded) {
            s_BubbleBaseModel = LoadModel("models/BubbleBase.glb");
            isModelLoaded = true;
        }

        static bool isShaderLoaded = false;
        if (!isShaderLoaded) {

            s_BubbleShader = LoadShader("shaders/bubbleBasic.vs", "shaders/bubbleBasic.fs");
            isShaderLoaded = true;
        }

        static bool isEnvironmentMapLoaded = false;
        if (!isEnvironmentMapLoaded) {
            Image img = LoadImage("textures/Level01_ReflectionMap.png");
            // Image img = LoadImage("textures/Cubemap.jpg");
            ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
            s_EnvironmentMap = LoadTextureCubemap(img, CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE);
            isEnvironmentMapLoaded = true;
        }
    }

private:
    void Init();

    void SetColor(BubbleType type);

    Vector3 GetRandomSpawnPos();
    void ApplyForces();
    void ClearForces();
    void ResolveCollision(Bubble* collider);
};
