#pragma once
#include "Entity.h"
#include "GameManager.h"
#include "Singleton.h"
#include "StaticMesh.h"
#include <memory>

class EffectManager : public Entity, public Singleton<EffectManager> {
  public:
    EffectManager();
    ~EffectManager();

    void ActivateElectroShield();
    void DeactivateElectroShield();

    void ActivateAnemoShield();
    void DeactivateAnemoShield();

    void Start() override;

    void Update(float dT = 1.0f) override;

    bool electroShieldAvailable = false;
    bool anemoShieldAvailable   = false;

  private:
    float m_Time;

    std::unique_ptr<StaticMesh> m_ElectroShieldMesh;
    int m_ElectroTimeId, m_ElectroBlinkId;
    bool m_ElectroBlink                = false;
    const float ELECTRO_BLINK_DURATION = 1.0f;

    std::unique_ptr<StaticMesh> m_AnemoShieldMesh;
    int m_AnemoTimeId;
    float anemoTime;

    void InitElectroShield();
    void InitAnemoShield();
};
