#include "Entity.h"
#include "StaticMesh.h"
#include <memory>
#include "ElementBubbleParticles.h"
class TestEntity : public Entity{
public:
    TestEntity(Scene* parentScene);
    ~TestEntity();

    void Update(float dT = 1.0f) override;

  private:
    // std::unique_ptr<StaticMesh> m_ElectroShieldMesh;
    // int m_ElectroTimeId, m_ElectroBlinkId;
    // int m_ElectroBlink                 = 0;
    // float electroShieldTimer           = 0.0f;
    // const float ELECTRO_BLINK_DURATION = 1.0f;
    std::unique_ptr<ElementBubbleParticles> electroParticles = nullptr;
    std::unique_ptr<ParticleSystem> smokeparticles = nullptr;
    // std::unique_ptr<StaticMesh> trailMesh = nullptr;
    int id = 0;
    
};

