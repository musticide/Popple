#include "EffectManager.h"
#include "GameManager.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "raylib.h"
#include "raymath.h"

EffectManager::EffectManager()
{
    LOGI("Effect Man Construct");

    LOGI("Effect Man Construct");
}

EffectManager::~EffectManager() { }

void EffectManager::ActivateElectroShield() {
    m_ElectroShieldMesh->SetActive(true);
}
void EffectManager::DeactivateElectroShield() { m_ElectroShieldMesh->SetActive(false); }

void EffectManager::Start() { 
    LOGI("Effect Man Start");
    m_ElectroShieldMesh
        = parentScene->CreateEntity<StaticMesh>(true, "models/BubbleBase_01.glb", RenderQueue::TRANSPARENT);
    m_ElectroShieldMesh->GetModel().materials[0].shader
        = *ResourceManager::GetShader("shaders/ElectroBubble_VS.glsl", "shaders/ElectroBubble_PS.glsl");
    m_ElectroShieldMesh->GetModel().materials[0].maps[0].texture
        = *ResourceManager::GetTexture("textures/ElectroBubble_UTIL.png");
    electroTimeId = GetShaderLocation(m_ElectroShieldMesh->GetModel().materials[0].shader, "_Time");
    m_ElectroShieldMesh->scale = Vector3Scale(Vector3One(), GameManager::Get().electroShieldRadius );
    m_ElectroShieldMesh->SetActive(false);
    LOGI("Effect Man Start");
}

void EffectManager::Update(float dT)
{
    float time = GetTime();
    if (m_ElectroShieldMesh->IsActive())
        SetShaderValue(m_ElectroShieldMesh->GetModel().materials[0].shader, electroTimeId, &time, SHADER_UNIFORM_FLOAT);
}
