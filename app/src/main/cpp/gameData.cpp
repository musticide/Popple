#include "gameData.h"
#include "Entity.h"
#include "Log.h"
#include "bubble.h"
#include "bubbleManager.h"
#include "raylib.h"


void GameData::DecreaseSpawnIntervalInternal(float factor)
{
    if (m_SpawnInterval > m_MinSpawnInterval)
        m_SpawnInterval -= factor;
    LOGI("Spawn Interval Decreased to %f", m_SpawnInterval);
}

void GameData::SetSpawnIntervalInternal(float interval) { m_SpawnInterval = interval; }

void GameData::Start() { 
    this->SetRenderQueue(RenderQueue::UI);
}

void GameData::Update(float dT)
{
    switch (m_ActiveElementalEffect) {
    case NO_ELEMENTAL_EFFECT:
        break;
    case ELECTRO:
        m_ElectroShieldTimer += dT;
        if (m_ElectroShieldTimer >= m_ShieldDuration) {
            m_ActiveElementalEffect = NO_ELEMENTAL_EFFECT;
            m_ElectroShieldTimer = 0.0f;
        }
        break;
    case ANEMO:
        m_AnemoEffectTimer += dT;
        if (m_AnemoEffectTimer >= m_AnemoEffectDuration) {
            m_AnemoEffectTimer = 0.0f;
            m_ActiveElementalEffect = NO_ELEMENTAL_EFFECT;
        }
        break;
    default:
        break;
    }
}

void GameData::AddSpecialBubbleInternal(BubbleType type)
{
    if (type != DEFAULT_BUBBLE) {
        for (int i = 1; i < BUBBLE_TYPES_COUNT; i++) {
            if (i == type)
                m_ComboCount[i]++;
            else
                m_ComboCount[i] = 0;

            LOGI("Combo Count %d = %d", i, m_ComboCount[i]);
        }

        switch (type) {
        case ELECTRO_BUBBLE:
            if (m_ComboCount[ELECTRO_BUBBLE] >= MAX_COMBO_LENGTH) {
                LOGI("Electro Shield Activated");
                m_ComboCount[ELECTRO_BUBBLE] = 0;
                m_ActiveElementalEffect = ELECTRO;
            }
            break;
        case ANEMO_BUBBLE:
            if (m_ComboCount[ANEMO_BUBBLE] >= MAX_COMBO_LENGTH) {
                LOGI("Anemo Activated");
                m_ComboCount[ANEMO_BUBBLE] = 0;
                m_ActiveElementalEffect = ANEMO;
            }
            break;
        default:
            LOGE("Bubble Type Out of range");
            break;
        }
    }
}

void GameData::ActivateAnemoBlast() { }

void GameData::DrawComboCountInternal() const
{
    for (int i = 0; i < MAX_COMBO_LENGTH; i++) {
        Color color = GRAY;
        float radius = 10;
        Vector2 center = { -80, (float)-GetScreenHeight() / 2 + 200 };
        center.x += 80 * i;

        for (int j = 1; j < BUBBLE_TYPES_COUNT; j++) {
            if (m_ComboCount[j] > 0 && i < m_ComboCount[j]) {
                color = GetElementalColorInternal((ElementalEffect)j);
                radius = 30;
            }
        }

        DrawCircleV(center, radius, color);
    }
}
Color GameData::GetElementalColorInternal(ElementalEffect type) const
{
    Color result;

    switch (type) {
    case NO_ELEMENTAL_EFFECT:
        result = WHITE;
        break;
    case ELECTRO:
        result = PURPLE;
        break;
    case ANEMO:
        result = { 107, 227, 183, 255 };
        break;
    default:
        result = WHITE;
        break;
    }

    return result;
}
void GameData::ResetInternal()
{
    m_SpawnInterval = 1.5f;
    m_MinSpawnInterval = 0.20f;

    m_ShieldDuration = 5.0f;
    m_ElectroShieldTimer = 0.0f;

    m_AnemoEffectTimer = 0.0f;
    m_AnemoEffectDuration = 1.0f;
    for (int j = 1; j < BUBBLE_TYPES_COUNT; j++) {
        m_ComboCount[j] = 0;
    }
}


