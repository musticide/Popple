#include "Tower.h"
#include "DrawableEntity.h"
#include "Log.h"
#include "ResourceManager.h"
#include "raylib.h"

Tower::Tower()
    : DrawableEntity(RenderQueue::OPAQUE)
    , position(Vector3Zero())
    , rotation({ 0.5f, 1.0f, 0.5f })
    , scale({ 2.0f, 2.0f, 2.0f })
    , color(BLUE)
    , rotate(0.0f)
{
    m_Model = ResourceManager::GetModel("models/TowerBase.glb");
    m_Model->materials[0].shader = *ResourceManager::GetShader("shaders/towerBasic.vs", "shaders/towerBasic.fs");
    SetShaderValue(m_Model->materials[0].shader, GetShaderLocation(m_Model->materials[0].shader, "environmentMap"),
        (int[1]) { MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);

    m_Model->materials[0].maps[MATERIAL_MAP_CUBEMAP].texture
        = *ResourceManager::GetCubemap("textures/Level01_ReflectionMap.png");
}

Tower::~Tower() { }

void Tower::Start()
{
    LOGV("Tower start");
    SetActive(true);
}

void Tower::Update(float dT)
{
    rotation = Vector3RotateByAxisAngle(rotation, { 0.5f, 1.0f, 0.5f }, 5.f);
    rotate += 0.8f;
}

void Tower::Draw() const
{
    if (m_Model.get() != nullptr)
        DrawModelEx(*m_Model, position, (Vector3) { 0.5f, 1.0f, 0.5f }, rotate, scale, color);
    else
        LOGW("Could not draw model");
}
