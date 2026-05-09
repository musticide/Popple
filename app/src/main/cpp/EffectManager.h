#pragma once
#include "Entity.h"
#include "GameManager.h"
#include "Singleton.h"
#include "StaticMesh.h"
#include <array>
#include <memory>

class EffectManager : public Entity, public Singleton<EffectManager> {
  public:
    EffectManager();
    ~EffectManager();

    void ChargeEffect(ElementType type);
    void DischargeEffect(ElementType type);
    bool IsEffectCharged(ElementType type);

    void Start() override;

    void Update(float dT = 1.0f) override;

    void ActivateEffect(ElementType type);
    bool IsEffectActive(ElementType type) {
        return effectActive[(int)type];
    }

    void Reset() {
        std::fill(effectCharged.begin(), effectCharged.end(), false);
        std::fill(effectActive.begin(), effectActive.end(), false);
    }

  private:
    float m_Time;

    std::unique_ptr<StaticMesh> m_ElectroShieldMesh;
    int m_ElectroTimeId, m_ElectroBlinkId;
    int m_ElectroBlink                 = 0;
    float electroShieldTimer           = 0.0f;
    const float ELECTRO_BLINK_DURATION = 1.0f;

    std::unique_ptr<StaticMesh> m_AnemoShieldMesh;
    int m_AnemoTimeId;
    float anemoEffectTimer = 0.0f;

    float cryoEffectTimer = 0.0f;

    std::array<bool, (int)ElementType::COUNT> effectCharged = { false };
    std::array<bool, (int)ElementType::COUNT> effectActive  = { false };

    void InitElectroShield();
    void ActivateElectroShield();
    void DeactivateElectroShield();

    void InitAnemoShield();
    void ActivateAnemoShield();
    void DeactivateAnemoShield();

    void InitCryoShield();
    void ActivateCryoShield();
    void DeactivateCryoShield();
};
