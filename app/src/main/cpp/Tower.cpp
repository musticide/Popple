#include "Tower.h"
#include "Log.h"
#include "raylib.h"

Tower::Tower()
    : position(Vector3Zero())
    , rotation({ 0.5f, 1.0f, 0.5f })
    , scale({ 2.0f, 2.0f, 2.0f })
    , color(BLUE)
    , rotate(0.0f)
{
}

Tower::~Tower() { }
void Tower::Start()
{
    Image img = LoadImage("textures/Level01_ReflectionMap.png");
    // Image img = LoadImage("textures/Cubemap.jpg");
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    TextureCubemap m_EnvironmentMap = LoadTextureCubemap(img, CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE);

    SetActive(true);
    m_Model = LoadModel("models/TowerBase.glb");
    m_Model.materials[0].shader = LoadShader("shaders/towerBasic.vs", "shaders/towerBasic.fs");
    SetShaderValue(m_Model.materials[0].shader, GetShaderLocation(m_Model.materials[0].shader, "environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
    m_Model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = m_EnvironmentMap;
}

void Tower::Update(float dT)
{
    rotation = Vector3RotateByAxisAngle(rotation, { 0.5f, 1.0f, 0.5f }, 5.f);
    rotate += 0.8f;
}

void Tower::Draw() const { DrawModelEx(m_Model, position, (Vector3) { 0.5f, 1.0f, 0.5f }, rotate, scale, color); }
