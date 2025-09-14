#include "bubble.h"
#include "input.h"
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

void Bubble::Start() { 
    if (!m_IsActive)
        return;
    Init();
}

void Bubble::Init()
{
    position = (Vector2) { (float)GetRandomValue(-2000, 2000), (float)GetRandomValue(-2000, 2000) };
    radius = GetRandomValue(50, 60);
    moveSpeed = (Vector2) { (float)GetRandomValue(1.0f, 3.0f), (float)GetRandomValue(1.0f, 3.0f) };
}

void Bubble::Update()
{
    if (!m_IsActive)
        return;

    for (int i = 0; i < GetTouchPointCount(); i++) {
        if (IsPointInBubble(Input::Get().GetTouchPositionWS(i))) {
            Init();
        }
    }

    if(Vector2Length(position) <= radius)
    {
        //Bubble is at the center
        //TODO: Call health decrease function
        Init();
    }

    Move();
}

void Bubble::Draw() const
{
    if (!m_IsActive)
        return;

    DrawCircleV(position, radius, color);
}

void Bubble::Move()
{
    Vector2 moveDir = Vector2Normalize(Vector2Zero() - position);
    position += moveDir * moveSpeed;
}

void Bubble::SetActive(bool active) { m_IsActive = active; }

bool Bubble::IsPointInBubble(Vector2 point) const { return Vector2Length(point - position) <= radius; }
