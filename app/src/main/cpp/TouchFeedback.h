#pragma once
#include "DrawableEntity.h"
#include "Entity.h"
#include "ParticleSystem.h"
#include "StaticMesh.h"
#include "raylib.h"
#include <memory>

class TouchFeedback : public Entity{
public:
    TouchFeedback();
    ~TouchFeedback();

    void Start() override;
    void Update(float dT = 1.0f) override;

    Color color = WHITE;
private:
    std::unique_ptr<ParticleSystem> touchParticles = nullptr;
    // std::unique_ptr<StaticMesh> sm_TestMesh = nullptr;
};
