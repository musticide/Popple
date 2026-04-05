#include "EffectManager.h"
#include "GameManager.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "raylib.h"
#include "raymath.h"

EffectManager::EffectManager() {
}

EffectManager::~EffectManager() {
}

void EffectManager::ActivateElectroShield() {
    m_ElectroShieldMesh->SetActive(true);
}
void EffectManager::DeactivateElectroShield() {
    m_ElectroShieldMesh->SetActive(false);
}

void EffectManager::Start() {
    m_ElectroShieldMesh =
    parentScene->CreateEntity<StaticMesh>(true, "models/BubbleBase_01.glb", RenderQueue::TRANSPARENT);
    m_ElectroShieldMesh->GetModel().materials[0].shader =
    *ResourceManager::GetShader("shaders/ElectroBubble_VS.glsl", "shaders/ElectroBubble_PS.glsl");
    m_ElectroShieldMesh->GetModel().materials[0].maps[0].texture =
    *ResourceManager::GetTexture("textures/ElectroBubble_UTIL.png");

    m_ElectroTimeId  = GetShaderLocation(m_ElectroShieldMesh->GetModel().materials[0].shader, "_Time");
    m_ElectroBlinkId = GetShaderLocation(m_ElectroShieldMesh->GetModel().materials[0].shader, "_Blink");
    m_ElectroShieldMesh->scale = Vector3Scale(Vector3One(), GameManager::Get().electroShieldRadius);
    m_ElectroShieldMesh->SetActive(false);

    GameManager::Get().activeElementEffectChanged.connect(
    [this](ElementType effect) { this->activeEffect = effect; });
}

void EffectManager::Update(float dT) {
    switch (activeEffect) {
    case ElementType::NONE: break;
    case ElementType::ELECTRO:
        if (m_ElectroShieldMesh->IsActive()) {

            // LOGI("%f timer, %f blink", GameManager::Get().electroShieldTimer,
            // GameManager::Get().ELECTRO_SHIELD_DURATION - ELECTRO_BLINK_DURATION);

            if (GameManager::Get().electroShieldTimer > GameManager::Get().ELECTRO_SHIELD_DURATION - ELECTRO_BLINK_DURATION) {
                m_ElectroBlink = true;
            } else {
                m_ElectroBlink = false;
            }
            SetShaderValue(m_ElectroShieldMesh->GetModel().materials[0].shader, m_ElectroBlinkId,
            &m_ElectroBlink, SHADER_UNIFORM_FLOAT);

            SetShaderValue(m_ElectroShieldMesh->GetModel().materials[0].shader, m_ElectroTimeId,
            &GameManager::Get().electroShieldTimer, SHADER_UNIFORM_FLOAT);
        }
        break;
    case ElementType::ANEMO: break;
    default: LOGE("Invalid Effect: Active effect index out of bounds"); break;
    }
}
