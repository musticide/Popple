#pragma once
#include "Entity.h"
#include "GameManager.h"
#include "Singleton.h"
#include "StaticMesh.h"

class EffectManager : public Entity, public Singleton<EffectManager> {
  public:
    EffectManager();
    ~EffectManager();

    void ActivateElectroShield();
    void DeactivateElectroShield();

    void Start() override;

    void Update(float dT = 1.0f) override;

    ElementType activeEffect = ElementType::NONE;

  private:
    float m_Time;

    std::unique_ptr<StaticMesh> m_ElectroShieldMesh;
    int m_ElectroTimeId, m_ElectroBlinkId;
    bool m_ElectroBlink = false;
    const float ELECTRO_BLINK_DURATION = 1.0f;
};
