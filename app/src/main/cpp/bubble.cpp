#include "bubble.h"
#include "raylib.h"
#include "raymath.h"

Bubble::Bubble()
    : position((Vector2) { 0, 0 })
    , rotation(0.0f)
    , radius(1.0f)
    , color(RAYWHITE)
{
}

Bubble::~Bubble() { }

void Bubble::Move()
{
    Vector2 moveDir = Vector2Normalize(Vector2Zero() - position);
    position += moveDir * moveSpeed;
}

void Bubble::Update()
{
    if (!m_IsActive)
        return;

    Move();
}

void Bubble::Draw() const
{
    if (!m_IsActive)
        return;

    DrawCircleV(position, radius, color);
}

void Bubble::SetActive(bool active) { m_IsActive = active; }

bool Bubble::IsPointInBubble(Vector2 point) const
{
    return Vector2Length(point - position) <= radius;

}
