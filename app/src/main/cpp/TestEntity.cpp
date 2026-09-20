#include "TestEntity.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "raylib.h"

TestEntity::TestEntity(Scene* parentScene)
: Entity(parentScene) {
    // m_ElectroShieldMesh =
    //     parentScene->CreateEntity<StaticMesh>(true, "models/ElectroShield.glb", RenderQueue::TRANSPARENT);
    // m_ElectroShieldMesh->GetModel().materials[0].shader =
    //     *ResourceManager::GetShader("shaders/ElectroBubble.vert", "shaders/ElectroBubble.frag");
    // m_ElectroShieldMesh->GetModel().materials[0].maps[0].texture =
    //     *ResourceManager::GetTexture("textures/ElectroBubble_UTIL.png");

    // m_ElectroTimeId  = GetShaderLocation(m_ElectroShieldMesh->GetModel().materials[0].shader, "_Time");
    // m_ElectroBlinkId = GetShaderLocation(m_ElectroShieldMesh->GetModel().materials[0].shader, "_Blink");
    // m_ElectroShieldMesh->scale = Vector3Scale(Vector3One(), GameData::electroShieldRadius);

    // electroParticles = parentScene->CreateEntity<ElementBubbleParticles>(true);
    // trailMesh = parentScene->CreateEntity<StaticMesh>(true, "models/BubbleTrail.glb",
    // RenderQueue::TRANSPARENT); trailMesh->GetModel().materials[0].shader =
    //     *ResourceManager::GetShader("shaders/particles.vert", "shaders/particles.frag");
    // id = GetShaderLocation(trailMesh->GetModel().materials[0].shader, "_tfrc");
    // trailMesh->GetModel().materials[0].maps[0].texture =
    //     *ResourceManager::GetTexture("textures/AnemoTrailSpritesheet.png");
    // // trailMesh->scale = { .4f, .1f, .1f };
    // trailMesh->tint  = (Color){ 85, 235, 152, 255 };
    smokeparticles                                    = parentScene->CreateEntity<ParticleSystem>(true, 10);
    smokeparticles->particleProperties.lifetime       = 0.85f;
    smokeparticles->particleProperties.startSize      = 0.25f;
    smokeparticles->particleProperties.endSize        = 0.0f;
    smokeparticles->particleProperties.sizeVariation  = 0.05f;
    smokeparticles->emitType                          = EmitType::CONTINUOUS;
    smokeparticles->emitRate                           = 5;
    smokeparticles->shape                             = EmitShape::NONE;
    smokeparticles->direction                         = { 0, 0, -1 };
    smokeparticles->particleProperties.initialSpeed   = 0.3f;
    smokeparticles->particleProperties.speedVariation = 0.0f;
    smokeparticles->particleProperties.damping        = 0.05f;
    smokeparticles->particleProperties.startColor     = (Color){ 85, 235, 152, 255 };
    smokeparticles->particleProperties.endColor       = BLANK;
    smokeparticles->model.materials[0].maps[0].texture = *ResourceManager::GetTexture("textures/Smoke15Frames.png");
    smokeparticles->textureProperties.fps              = 12;
    smokeparticles->textureProperties.rows             = 2;
    smokeparticles->textureProperties.columns          = 2;
    smokeparticles->textureProperties.isSpriteSheet    = true;
    smokeparticles->localSpace                         = true;

    smokeparticles->endPoint      = { 0, 0, 0 };
    smokeparticles->endPointForce = 1.0f;
}

TestEntity::~TestEntity() {
}
void TestEntity::Update(float dT) {
    Vector4 tfrc = { (float)GetTime(), 12.f, 8.f, 2.f };
    // SetShaderValue(trailMesh->GetModel().materials[0].shader, id, &tfrc, SHADER_UNIFORM_VEC4);
    // electroShieldTimer += dT;
    // if (electroShieldTimer > GameData::ELECTRO_SHIELD_DURATION) {
    //     electroShieldTimer = 0.0f;
    //     m_ElectroBlink     = 0;
    // }
    // if (electroShieldTimer > GameData::ELECTRO_SHIELD_DURATION - ELECTRO_BLINK_DURATION) {
    //     m_ElectroBlink = 1;
    // } else {
    //     m_ElectroBlink = 0;
    // }
    // if (m_ElectroBlinkId >= 0) {
    //     SetShaderValue(m_ElectroShieldMesh->GetModel().materials[0].shader, m_ElectroBlinkId, &m_ElectroBlink, SHADER_UNIFORM_INT);
    // }

    // if (m_ElectroTimeId >= 0) {
    //     SetShaderValue(m_ElectroShieldMesh->GetModel().materials[0].shader, m_ElectroTimeId, &electroShieldTimer, SHADER_UNIFORM_FLOAT);
    // }
}
