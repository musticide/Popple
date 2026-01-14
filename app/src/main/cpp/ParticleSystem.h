#pragma once
#include "DrawableEntity.h"
#include "Entity.h"
#include "editor/NetworkReceiver.h"
#include "raylib.h"
#include "raymath.h"
#include <memory>
#include <vector>

struct ParticleProperties {
    // Particle
    float lifetime = 1.0f;
    float initialSpeed = 0.0f;
    float size = 1.0f;
    float sizeVariation = 0.0f;

    // Rendering
    Color startColor = WHITE;
    Color endColor = WHITE;
    /// uses default quad if not specified
};

struct Particle {
    bool isActive = false;
    float lifetime = 1.0f;
    float age = 0.0f;

    Vector3 velocity = (Vector3) { 0, 0, 0 };
    Vector3 position = (Vector3) { 0, 0, 0 };
    float size = 1.0f;

    Color color = WHITE;
};

enum class EmitShape { NONE, CIRCLE };

enum class EmitType { CONTINUOUS, BURST };

class ParticleSystem : public DrawableEntity {
public:
    ParticleSystem(int maxParticles);
    ~ParticleSystem();

    ParticleProperties particleProperties;
    Vector3 position;

    /// Default Continuous
    EmitType emitType = EmitType::CONTINUOUS;
    /// Rate of emission per second
    int emitRate = 2;
    int maxParticles = 50;

    EmitShape shape;
    /// Used when shape NONE
    Vector3 direction = Vector3Zero();

    Model model;
    /// uses default material if not specified
    Material material;

#ifdef DEBUG
    void ApplyEditorParams(const json& data);
    json GetCurrentParams() const;
#endif

    void Start() override;
    void Update(float dT = 1.0f) override;
    void Draw() const override;

    void Burst(int amount);

private:
    std::vector<Particle> m_ParticlePool;
    int m_CurrentIndex;
    float m_TimeElapsed;

    void Emit();
    Vector3 GetCircularDirection();
};
