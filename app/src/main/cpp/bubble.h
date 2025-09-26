#pragma once

#include "raylib.h"
#include "raymath.h"
#include <raymob.h>
#include <vector>

#include "Entity.h"
#include "input.h"

enum BubbleType { DEFAULT_BUBBLE = 0, ELECTRO_BUBBLE, ANEMO_BUBBLE, BUBBLE_TYPES_COUNT };

class Bubble : public Entity {
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

    void AddForce(Vector2 force);

    void Spawn();

    bool IsPointInBubble(Vector2 point) const;

    // Getters
    Vector2 GetVelocity() const { return m_Velocity; }
    BubbleType GetType() const { return m_Type; }

private:
    BubbleType m_Type;
    Vector2 m_Velocity;
    constexpr static const float m_Drag = 0.1;
    constexpr static const float m_CenterForce = 0.5;
    bool m_IsActive = true;
    // std::vector<Vector2> m_Forces;

    void Init();

    void SetColor(BubbleType type);

    Vector2 GetRandomSpawnPos();
    void ApplyForces();
    void ClearForces();
    void ResolveCollision(Bubble* collider);
};
