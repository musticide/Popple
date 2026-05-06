#include "EffectManager.h"
#include "DrawableEntity.h"
#include "GameManager.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "bubbleManager.h"
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
    if (effectActive[(int)ElementType::ELECTRO]) {
        electroShieldTimer += dT;

        if (electroShieldTimer > GameData::ELECTRO_SHIELD_DURATION) {
            effectActive[(int)ElementType::ELECTRO] = false;
            electroShieldTimer                      = 0.0f;
            DeactivateElectroShield();
        }

        if (m_ElectroShieldMesh && m_ElectroShieldMesh->IsActive()) {

            if (electroShieldTimer > GameData::ELECTRO_SHIELD_DURATION - ELECTRO_BLINK_DURATION) {
                m_ElectroBlink = true;
            } else {
                m_ElectroBlink = false;
            }
            if (m_ElectroBlinkId >= 0) {
                SetShaderValue(m_ElectroShieldMesh->GetModel().materials[0].shader, m_ElectroBlinkId, &m_ElectroBlink, SHADER_UNIFORM_FLOAT);
            }

            if (m_ElectroTimeId >= 0) {
                SetShaderValue(m_ElectroShieldMesh->GetModel().materials[0].shader, m_ElectroTimeId, &electroShieldTimer, SHADER_UNIFORM_FLOAT);
            }
        }
    }
    if (effectActive[(int)ElementType::ANEMO]) {
        anemoEffectTimer += dT;
        if (anemoEffectTimer > GameData::ANEMO_EFFECT_DURATION) {
            effectActive[(int)ElementType::ANEMO] = false;
            BubbleManager::Get().ContinueSpawn();
            EffectManager::Get().DeactivateAnemoShield();
            anemoEffectTimer = 0.0f;
            LOGI("Anemo Deactivated");
        }
        if (m_AnemoShieldMesh && m_AnemoShieldMesh->IsActive()) {
            anemoTime += dT;
            m_AnemoShieldMesh->scale *= 1.0f + (6.5 * dT);
            if (m_AnemoTimeId >= 0) {
                SetShaderValue(m_AnemoShieldMesh->GetModel().materials[0].shader, m_AnemoTimeId, &anemoTime, SHADER_UNIFORM_FLOAT);
            }
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
    m_ElectroShieldMesh->scale = Vector3Scale(Vector3One(), GameData::electroShieldRadius);
    m_ElectroShieldMesh->SetActive(false);
}

void EffectManager::ActivateElectroShield() {
    LOGI("Electro Shield Activated");
    m_ElectroShieldMesh->SetActive(true);
    ActivateEffect(ElementType::ELECTRO);
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
    m_AnemoShieldMesh->scale = { 2.f, 2.f, 2.f };
    m_AnemoShieldMesh->SetActive(false);
}

void EffectManager::ActivateAnemoShield() {
    LOGI("EM: Anemo Shield Activated");
    m_AnemoShieldMesh->SetActive(true);
}

void EffectManager::DeactivateAnemoShield() {
    LOGI("EM: Deactivate Anemo Shield");
    if (this->m_AnemoShieldMesh) {
        this->m_AnemoShieldMesh->scale = { 1.f, 1.f, 1.f };
        this->m_AnemoShieldMesh->SetActive(false);
    }
}
void EffectManager::ActivateEffect(ElementType type) {
    if (!effectCharged[(int)type]) return;
    effectActive[(int)type]  = true;
    effectCharged[(int)type] = false;
    GameManager::Get().ResetComboCount(type);

    switch (type) {

        case ElementType::NONE:
            break;
        case ElementType::ELECTRO:
            ActivateElectroShield();
            break;
        case ElementType::ANEMO:
            ActivateAnemoShield();
            BubbleManager::Get().PauseSpawn();
            break;
        default:
            break;
    }
}
void EffectManager::ChargeEffect(ElementType type) {
    effectCharged[(int)type] = true;
}

void EffectManager::DischargeEffect(ElementType type) {
    effectCharged[(int)type] = false;
}

bool EffectManager::IsEffectCharged(ElementType type) {
    return effectCharged[(int)type];
}

