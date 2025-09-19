#include "bubble.h"
#include "Log.h"
#include "raylib.h"
#include "raymath.h"
#include "spatialGrid.h"

Bubble::Bubble()
    : position((Vector2) { 0, 0 })
    , rotation(0.0f)
    , radius(1.0f)
    , color(RAYWHITE)
{
}

Bubble::~Bubble() { }

void Bubble::Start()
{
    if (!m_IsActive)
        return;
    Init();
}

Vector2 Bubble::GetRandomSpawnPos()
{
    float distance = GetRandomValue(2200, 2000);
    float randAngle = GetRandomValue(0, 360);
    return (Vector2) { (float)cos(randAngle) * distance, (float)sin(randAngle) * distance };
}

void Bubble::Init()
{
    ClearForces();
    m_Velocity = Vector2Zero();

    position = GetRandomSpawnPos();
    radius = GetRandomValue(50, 60);
    AddForce(Vector2Scale(Vector2Normalize(Vector2Zero() - position),m_CenterForce));
}

void Bubble::ResolveCollision(Bubble* collider)
{
    Vector2 aMinusB = this->position - collider->position;
    float totalRadius = this->radius + collider->radius;
    float distance = Vector2Length(aMinusB);
    float penetration = totalRadius - distance;

    Vector2 collisionNormal = Vector2Normalize(aMinusB);///distance);
    this->position += Vector2Scale(collisionNormal , penetration * 0.5f);
    collider->position -= Vector2Scale(collisionNormal , penetration * 0.5f);
}

void Bubble::Update(float dT)
{
    if (!m_IsActive)
        return;

    // INFO: Respawn as new on touch
    for (int i = 0; i < GetTouchPointCount(); i++) {
        if (IsPointInBubble(Input::Get().GetTouchPositionWS(i))) {
            Init();
        }
    }

    // INFO: Respawn as new on reaching center
    if (Vector2Length(position) <= radius + 50) {
        // TODO: Call health decrease function
        Init();
    }

    SpatialGrid::AddEntity(this);

    // Pull towards center
    AddForce(Vector2Scale(Vector2Normalize(Vector2Zero() - position),m_CenterForce));

    ApplyForces();

    // INFO: Check for collisions
    for (Bubble* nearby : SpatialGrid::GetNearbyEntities(this->position)) {
        if (CheckCollisionCircles(this->position, this->radius, nearby->position, nearby->radius)) {
            ResolveCollision(nearby);
        }
    }

}

void Bubble::Draw() const
{
    if (!m_IsActive)
        return;

    DrawCircleV(position, radius, color);
}

void Bubble::SetActive(bool active) { m_IsActive = active; }

bool Bubble::IsPointInBubble(Vector2 point) const { return Vector2Length(point - position) <= radius; }

void Bubble::AddForce(Vector2 force)
{
    m_Velocity += force;
}

void Bubble::ApplyForces()
{
    if (Vector2Length(m_Velocity) > maxMoveSpeed) {
        m_Velocity = Vector2Normalize(m_Velocity) * maxMoveSpeed;
    }

    m_Velocity = Vector2Lerp(m_Velocity, Vector2Zero(), m_Drag);

    position += m_Velocity;
}
void Bubble::ClearForces() { m_Forces.clear(); }
