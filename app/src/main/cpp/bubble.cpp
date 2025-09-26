#include "bubble.h"
#include "Log.h"
#include "bubbleManager.h"
#include "functionLibrary.h"
#include "gameData.h"
#include "raylib.h"
#include "raymath.h"
#include "score.h"
#include "spatialGrid.h"

Bubble::Bubble()
    : position((Vector2) { 0, 0 })
    , rotation(0.0f)
    , radius(1.0f)
    , color(RAYWHITE)
{
}

Bubble::~Bubble() { }

void Bubble::Start() { SetActive(false); }

Vector2 Bubble::GetRandomSpawnPos()
{
    int screenSize = GetScreenWidth() > GetScreenHeight() ? GetScreenWidth() : GetScreenHeight();
    screenSize /= 2;
    float distance = GetRandomValue(screenSize, screenSize * 1.05);
    float randAngle = GetRandomValue(0, 360);
    return (Vector2) { (float)cos(randAngle) * distance, (float)sin(randAngle) * distance };
}

void Bubble::Spawn() { Init(); }

void Bubble::Init()
{
    ClearForces();
    m_Velocity = Vector2Zero();

    position = GetRandomSpawnPos();
    radius = GetRandomValue(85, 70);
    AddForce(Vector2Scale(Vector2Normalize(Vector2Zero() - position), m_CenterForce));

    // 20% chance of a special bubble
    if (RollPercentage(40)) {
        m_Type = RollPercentage(50) ? ANEMO_BUBBLE : ELECTRO_BUBBLE;
    } else {
        m_Type = DEFAULT_BUBBLE;
    }

    SetColor(m_Type);
}

void Bubble::ResolveCollision(Bubble* collider)
{
    Vector2 aMinusB = this->position - collider->position;
    float totalRadius = this->radius + collider->radius;
    float distance = Vector2Length(aMinusB);
    float penetration = totalRadius - distance;

    Vector2 collisionNormal = Vector2Normalize(aMinusB); /// distance);
    this->position += Vector2Scale(collisionNormal, penetration * 0.5f);
    collider->position -= Vector2Scale(collisionNormal, penetration * 0.5f);
}

void Bubble::Update(float dT)
{

    SpatialGrid::AddEntity(this);

    // INFO: Pull towards center
    AddForce(Vector2Scale(Vector2Normalize(Vector2Zero() - position), m_CenterForce));

    ApplyForces();

    // INFO: Check for collisions
    for (Bubble* nearby : SpatialGrid::GetNearbyEntities(this->position)) {
        if (CheckCollisionCircles(this->position, this->radius, nearby->position, nearby->radius)) {
            ResolveCollision(nearby);
        }
    }
}

void Bubble::Draw() const { DrawCircleV(position, radius, color); }

bool Bubble::IsPointInBubble(Vector2 point) const { return Vector2Length(point - position) <= radius; }

void Bubble::AddForce(Vector2 force) { m_Velocity += force; }

void Bubble::ApplyForces()
{
    m_Velocity *= moveSpeed;
    if (Vector2Length(m_Velocity) > maxMoveSpeed) {
        m_Velocity = Vector2Normalize(m_Velocity) * maxMoveSpeed;
    }

    m_Velocity = Vector2Lerp(m_Velocity, Vector2Zero(), m_Drag);

    position += m_Velocity;
}
void Bubble::ClearForces() { }
void Bubble::SetColor(BubbleType type)
{
    switch (m_Type) {
    case DEFAULT_BUBBLE:
        color = WHITE;
        break;
    case ELECTRO_BUBBLE:
        color = PURPLE;
        break;
    case ANEMO_BUBBLE:
        color = { 107, 227, 183, 255 };
        break;
    default:
        color = WHITE;
        break;
    }
}

