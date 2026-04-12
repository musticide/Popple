#include "EffectManager.h"
#include "DrawableEntity.h"
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

void EffectManager::Start() {
    InitElectroShield();
    InitAnemoShield();

    LOGI("effect manager start");
}

void EffectManager::Update(float dT) {
    if (m_ElectroShieldMesh && m_ElectroShieldMesh->IsActive()) {

        if (GameManager::Get().electroShieldTimer > GameManager::Get().ELECTRO_SHIELD_DURATION - ELECTRO_BLINK_DURATION) {
            m_ElectroBlink = true;
        } else {
            m_ElectroBlink = false;
        }
        if (m_ElectroBlinkId >= 0) {
            SetShaderValue(m_ElectroShieldMesh->GetModel().materials[0].shader, m_ElectroBlinkId,
                &m_ElectroBlink, SHADER_UNIFORM_FLOAT);
        }

        if (m_ElectroTimeId >= 0) {
            SetShaderValue(m_ElectroShieldMesh->GetModel().materials[0].shader, m_ElectroTimeId,
                &GameManager::Get().electroShieldTimer, SHADER_UNIFORM_FLOAT);
        }
    }

    if (m_AnemoShieldMesh && m_AnemoShieldMesh->IsActive()) {
        anemoTime += dT;
        m_AnemoShieldMesh->scale *= 1.0f + (5.5 * dT);
        if (m_AnemoTimeId >= 0) {
            SetShaderValue(m_AnemoShieldMesh->GetModel().materials[0].shader, m_AnemoTimeId,
                &anemoTime, SHADER_UNIFORM_FLOAT);
        }
    }
}

void EffectManager::InitElectroShield() {

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
}

void EffectManager::ActivateElectroShield() {
    m_ElectroShieldMesh->SetActive(true);
}
void EffectManager::DeactivateElectroShield() {
    m_ElectroShieldMesh->SetActive(false);
}

void EffectManager::InitAnemoShield() {
    m_AnemoShieldMesh = parentScene->CreateEntity<StaticMesh>(true, "models/BubbleBase_01.glb", RenderQueue::TRANSPARENT);
    m_AnemoShieldMesh->GetModel().materials[0].shader =
        *ResourceManager::GetShader("shaders/AnemoBubble_VS.glsl", "shaders/AnemoBubble_PS.glsl");
    m_AnemoShieldMesh->GetModel().materials[0].maps[0].texture =
        *ResourceManager::GetTexture("textures/AnemoBubble_UTIL.png");

    m_AnemoTimeId            = GetShaderLocation(m_AnemoShieldMesh->GetModel().materials[0].shader, "_Time");
    m_AnemoShieldMesh->scale = { 1.f, 1.f, 1.f };
    m_AnemoShieldMesh->SetActive(false);
}

void EffectManager::ActivateAnemoShield() {
    m_AnemoShieldMesh->SetActive(true);
    LOGV("EM: Anemo Shield Activated");
}

void EffectManager::DeactivateAnemoShield() {
    LOGI("EM: Deactivate Anemo Shield");
        if (this->m_AnemoShieldMesh) {
            this->m_AnemoShieldMesh->scale = { 1.f, 1.f, 1.f };
            this->m_AnemoShieldMesh->SetActive(false);
    }
}
