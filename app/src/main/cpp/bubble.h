#pragma once

#include "raylib.h"
#include "raymath.h"
#include <raymob.h>
#include <vector>

#include "input.h"
#include "Entity.h"

enum BubbleType
{
    DEFAULT_BUBBLE,
    ELECTRO_BUBBLE,
    ANEMO_BUBBLE
};

class Bubble : public Entity{
public:
    Bubble();
    ~Bubble();

    Vector2 position;
    float rotation;
    float radius;
    Color color;
    float maxMoveSpeed = 15;
    float moveSpeed = 1.0;

    void Start() override;
    void Update(float dT = 1.0f) override;

    void Draw() const override;

    // void SetActive(bool active);

    void AddForce(Vector2 force);

    void Spawn();

    // Getters
    Vector2 GetVelocity() const { return m_Velocity; }

private:
    BubbleType type;
    Vector2 m_Velocity;
    constexpr static const float m_Drag = 0.1;
    constexpr static const float m_CenterForce = 0.5;
    bool m_IsActive = true;
    // std::vector<Vector2> m_Forces;

    void Init();
    bool IsPointInBubble(Vector2 point) const;

    void OnBubblePopped();
    Vector2 GetRandomSpawnPos();
    void ApplyForces();
    void ClearForces();
    void ResolveCollision(Bubble* collider);
};
