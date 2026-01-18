#pragma once
#include "Entity.h"
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

private:
    std::unique_ptr<StaticMesh> m_ElectroShieldMesh;
    int electroTimeId;
};
