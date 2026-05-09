#include "EffectManager.h"
#include "DrawableEntity.h"
#include "GameManager.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "bubbleManager.h"
#include <raymob.h>
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
            m_ElectroBlink                          = false;
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
            BubbleManager::Get().AnemoPushBack(false);
            EffectManager::Get().DeactivateAnemoShield();
            anemoEffectTimer = 0.0f;
            LOGI("Anemo Deactivated");
        }
        if (m_AnemoShieldMesh && m_AnemoShieldMesh->IsActive()) {
            m_AnemoShieldMesh->scale *= 1.0f + (6.5 * dT);
            if (m_AnemoTimeId >= 0) {
                SetShaderValue(m_AnemoShieldMesh->GetModel().materials[0].shader, m_AnemoTimeId, &anemoEffectTimer, SHADER_UNIFORM_FLOAT);
            }
        }
    }
    if (effectActive[(int)ElementType::CRYO]) {
        cryoEffectTimer += dT;
        if (cryoEffectTimer > GameData::CRYO_SHIELD_DURATION) {
            effectActive[(int)ElementType::CRYO] = false;
            BubbleManager::Get().CryoFreeze(false);
            cryoEffectTimer = 0.0f;
            DeactivateCryoShield();
        }
    }
}

void EffectManager::InitElectroShield() {

    m_ElectroShieldMesh =
        parentScene->CreateEntity<StaticMesh>(true, "models/ElectroShield.glb", RenderQueue::TRANSPARENT);
    // parentScene->CreateEntity<StaticMesh>(true, "models/BubbleBase_01.glb", RenderQueue::TRANSPARENT);
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
    VibrateMS(200);
    LOGI("Electro Shield Activated");
    m_ElectroShieldMesh->SetActive(true);
    ActivateEffect(ElementType::ELECTRO);
}
void EffectManager::DeactivateElectroShield() {
    VibrateMS(400);
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
    VibrateMS(200);
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
            BubbleManager::Get().AnemoPushBack(true);
            break;
        case ElementType::CRYO:
            ActivateCryoShield();
            BubbleManager::Get().CryoFreeze(true);
        default:
            break;
    }
}
void EffectManager::ChargeEffect(ElementType type) {
    VibrateMS(200);
    effectCharged[(int)type] = true;
}

void EffectManager::DischargeEffect(ElementType type) {
    effectCharged[(int)type] = false;
}

bool EffectManager::IsEffectCharged(ElementType type) {
    return effectCharged[(int)type];
}

void EffectManager::InitCryoShield() {
}

void EffectManager::ActivateCryoShield() {
    VibrateMS(200);
}

void EffectManager::DeactivateCryoShield() {
    VibrateMS(400);
}
