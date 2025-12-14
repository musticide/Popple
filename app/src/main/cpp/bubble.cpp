#include "bubble.h"
#include "Entity.h"
#include "GameManager.h"
#include "Log.h"
#include "ResourceManager.h"
#include "functionLibrary.h"
#include <raymob.h>
#include "raymath.h"
#include "spatialGrid.h"
#include <memory>

Bubble::Bubble()
    : position((Vector3) { 0, 0 })
    , rotation(0.0f)
    , radius(1.0f)
    , color(RAYWHITE)
{
    m_BubbleBaseModel = ResourceManager::GetModel("models/BubbleBase.glb");
    m_BubbleBaseModel->materials[0].shader
        = *ResourceManager::GetShader("shaders/bubbleBasic.vs", "shaders/bubbleBasic.fs");
    SetShaderValue(m_BubbleBaseModel->materials[0].shader,
        GetShaderLocation(m_BubbleBaseModel->materials[0].shader, "environmentMap"), (int[1]) { MATERIAL_MAP_CUBEMAP },
        SHADER_UNIFORM_INT);
    m_BubbleBaseModel->materials[0].maps[MATERIAL_MAP_CUBEMAP].texture
        = *ResourceManager::GetCubemap("textures/Level01_ReflectionMap.png");
    SetRenderQueue(RenderQueue::TRANSPARENT);
}

Bubble::~Bubble() { }

void Bubble::Start() { SetActive(false); }

Vector3 Bubble::GetRandomSpawnPos()
{
    //TODO: decrease distance
    float distance = GetRandomValue(40, 35);
    float randAngle = GetRandomValue(0, 360);
    return (Vector3) { (float)cos(randAngle) * distance, 0, (float)sin(randAngle) * distance };
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
        m_Type = RollPercentage(50) ? ElementType::ANEMO : ElementType::ELECTRO;
    } else {
        m_Type = ElementType::NONE;
    }

    SetColor();
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
            // ResolveCollision(nearby);
        }
    }
}

void Bubble::Draw() const
{
    if (m_BubbleBaseModel != nullptr)
        DrawModel(*m_BubbleBaseModel, position, radius, color);
}

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
void Bubble::SetColor()
{
    switch (m_Type) {

    case ElementType::NONE:
        color = WHITE;
        break;
    case ElementType::ELECTRO:
        color = PURPLE;
        break;
    case ElementType::ANEMO:
        color = { 107, 227, 183, 255 };
        break;
    case ElementType::COUNT:
    default:
        color = WHITE;
        break;
    }
    /* switch (m_Type) {
    case DEFAULT_BUBBLE:
    case ELECTRO_BUBBLE:
    case ANEMO_BUBBLE:
    default:
    } */
}
