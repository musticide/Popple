#include "bubble.h"
#include "Log.h"
#include "bubbleManager.h"
#include "functionLibrary.h"
#include "gameData.h"
#include "raylib.h"
#include "raymath.h"
#include "score.h"
#include "spatialGrid.h"

Model Bubble::s_BubbleBaseModel;
Shader Bubble::s_BubbleShader;

Bubble::Bubble()
    : position((Vector3) { 0, 0 })
    , rotation(0.0f)
    , radius(1.0f)
    , color(RAYWHITE)
{
}

Bubble::~Bubble() { }

void Bubble::Start()
{
    SetActive(false);
    m_BubbleBaseModel = s_BubbleBaseModel;
    m_BubbleBaseModel.materials[0].shader = s_BubbleShader;
}

Vector3 Bubble::GetRandomSpawnPos()
{
    int screenSize = GetScreenWidth() > GetScreenHeight() ? GetScreenWidth() : GetScreenHeight();
    screenSize /= 2;
    float distance = GetRandomValue(45, 55);
    float randAngle = GetRandomValue(0, 360);
    return (Vector3) { (float)cos(randAngle) * distance, 0, (float)sin(randAngle) * distance };
    // return Vector3{10, 0, 10};
}

void Bubble::Spawn() { Init(); }

void Bubble::Init()
{
    ClearForces();
    m_Velocity = Vector3Zero();

    position = GetRandomSpawnPos();
    radius = GetRandomValue(20, 25) / 10.0f;
    AddForce(Vector3Scale(Vector3Normalize(Vector3Zero() - position), m_CenterForce));

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
    Vector3 aMinusB = this->position - collider->position;
    float totalRadius = this->radius + collider->radius;
    float distance = Vector3Length(aMinusB);
    float penetration = totalRadius - distance;

    Vector3 collisionNormal = Vector3Normalize(aMinusB); /// distance);
    this->position += Vector3Scale(collisionNormal, penetration * 0.5f);
    collider->position -= Vector3Scale(collisionNormal, penetration * 0.5f);
}

void Bubble::Update(float dT)
{
    SpatialGrid::AddEntity(this);

    // INFO: Pull towards center
    AddForce(Vector3Scale(Vector3Normalize(Vector3Zero() - position), m_CenterForce));

    ApplyForces();

    // INFO: Check for collisions
    for (Bubble* nearby : SpatialGrid::GetNearbyEntities(this->position)) {
        if (CheckCollisionSpheres(this->position, this->radius, nearby->position, nearby->radius)) {
            ResolveCollision(nearby);
        }
    }
}

void Bubble::Draw() const { DrawModel(m_BubbleBaseModel, position, radius, color); }

bool Bubble::IsPointInBubble(Vector3 point) const
{
    return Vector3Length((Vector3) { point.x, 0, point.z } - position) <= radius;
}

void Bubble::AddForce(Vector3 force) { m_Velocity += force; }

void Bubble::ApplyForces()
{
    m_Velocity *= moveSpeed;
    if (Vector3Length(m_Velocity) > maxMoveSpeed) {
        m_Velocity = Vector3Normalize(m_Velocity) * maxMoveSpeed;
    }

    m_Velocity = Vector3Lerp(m_Velocity, Vector3Zero(), m_Drag);

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
