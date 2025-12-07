#pragma once

#include "Entity.h"
#include "raylib.h"
#include "raymath.h"
#include <memory>
#include <vector>

struct ParticleProperties {
    // Particle
    float lifetime = 1.0f;
    float initialSpeed = 0.0f;
    float size = 1.0f;
    float sizeVariation;

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

enum class EmitShape{
    NONE,
    CIRCLE
};

enum class EmitType{
    CONTINUOUS,
    BURST
};

class ParticleSystem : public Entity {
public:
    ParticleSystem(int maxParticles);
    ~ParticleSystem();

    ParticleProperties particleProperties;
    Vector3 position;

    ///Default Continuous
    EmitType emitType = EmitType::CONTINUOUS;
    /// Rate of emission per second
    int emitRate = 2;
    int maxParticles = 50;

    EmitShape shape;
    ///Used when shape NONE
    Vector3 direction = Vector3Zero();


    // std::shared_ptr<Model> model;
    Model model;
    /// uses default material if not specified
    Material material;

    void Start() override;
    void Update(float dT = 1.0f) override;
    void Draw() const override;

    void Burst(int amount);

private:
    std::vector<Particle> m_ParticlePool;
    int m_CurrentIndex;
    float m_TimeElapsed;

    Vector3 GetCircularDirection()
    {
        float randAngle = GetRandomValue(0, 360);
        return (Vector3) { (float)cos(randAngle), 0, (float)sin(randAngle) };
    }
    void Emit();
};
