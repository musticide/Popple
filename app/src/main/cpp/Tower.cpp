#include "Tower.h"
#include "Log.h"
#include "raylib.h"

Tower::Tower()
    : position(Vector3Zero())
    , rotation({ 0.5f, 1.0f, 0.5f })
    , scale({2.0f, 2.0f, 2.0f})
    , color(BLUE)
    , rotate(0.0f)
{
}

Tower::~Tower() { }
void Tower::Start()
{
    SetActive(true);
    m_Model = LoadModel("models/TowerBase.glb");
    m_Model.materials[0].shader = LoadShader("shaders/towerBasic.vs", "shaders/towerBasic.fs");
}

void Tower::Update(float dT)
{
    rotation = Vector3RotateByAxisAngle(rotation, {0.5f, 1.0f, 0.5f}, 5.f);
    rotate += 0.8f;
}

void Tower::Draw() const
{
    DrawModelEx(m_Model, position, (Vector3){0.5f,1.0f,0.5f}, rotate, scale, color);
}

