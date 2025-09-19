#pragma once

#include "raylib.h"
#include "raymath.h"
#include <raymob.h>
#include <vector>

#include "input.h"

class Bubble {
public:
    Bubble();
    ~Bubble();

    Vector2 position;
    float rotation;
    float radius;
    Color color;
    float maxMoveSpeed = 25;
    float moveSpeed = 1.0;

    void Start();
    void Update(float dT = 1.0f);

    void Draw() const;

    void SetActive(bool active);

    void AddForce(Vector2 force);

    void Spawn();

    // Getters
    bool IsActive() const { return m_IsActive; }
    Vector2 GetVelocity() const { return m_Velocity; }

private:
    bool m_IsActive = true;
    Vector2 m_Velocity;
    constexpr static const float m_Drag = 0.1;
    constexpr static const float m_CenterForce = 0.5;
    std::vector<Vector2> m_Forces;

    void Init();
    bool IsPointInBubble(Vector2 point) const;

    Vector2 GetRandomSpawnPos();
    void ApplyForces();
    void ClearForces();
    void ResolveCollision(Bubble* collider);
};
