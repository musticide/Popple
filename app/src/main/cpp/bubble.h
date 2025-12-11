#pragma once

#include "GameManager.h"
#include "Log.h"
#include <raymob.h>
#include "raymath.h"
#include <vector>

#include "Entity.h"
#include "input.h"

// enum BubbleType { DEFAULT_BUBBLE = 0, ELECTRO_BUBBLE, ANEMO_BUBBLE, BUBBLE_TYPES_COUNT };

class Bubble : public Entity {
private:
    bool m_IsActive = true;

    // Physics
    ElementType m_Type;
    Vector3 m_Velocity;
    constexpr static const float m_Drag = 0.1;
    constexpr static const float m_CenterForce = 0.01;
    std::shared_ptr<Model> m_BubbleBaseModel;

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
    ElementType GetType() const { return m_Type; }

private:
    void Init();

    void SetColor();

    Vector3 GetRandomSpawnPos();
    void ApplyForces();
    void ClearForces();
    void ResolveCollision(Bubble* collider);
};
