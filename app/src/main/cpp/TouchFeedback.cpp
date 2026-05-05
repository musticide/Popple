#include "TouchFeedback.h"
#include "Entity.h"
#include "Log.h"
#include "ParticleSystem.h"
#include "Scene.h"
#include "input.h"
#include "raylib.h"

TouchFeedback::TouchFeedback()
{
    // touchParticles.SetActive(true);
}

TouchFeedback::~TouchFeedback() { }

void TouchFeedback::Start()
{
    touchParticles = parentScene->CreateEntity<ParticleSystem>(true, 15);

    touchParticles->particleProperties.lifetime = 0.2f;
    touchParticles->particleProperties.startSize = 0.08f;
    touchParticles->particleProperties.endSize = 0.02f;
    touchParticles->particleProperties.sizeVariation = 0.05f;
    touchParticles->emitType = EmitType::BURST;
    touchParticles->shape = EmitShape::CIRCLE;
    touchParticles->particleProperties.initialSpeed = 0.6f;
    touchParticles->particleProperties.speedVariation = 0.1f;
    touchParticles->particleProperties.startColor = color;
    touchParticles->particleProperties.endColor = {255, 255, 255, 0};
}

void TouchFeedback::Update(float dT)
{
    if (touchParticles != nullptr) {
        if (GetTouchPointCount() > 0) {
            touchParticles->position = Input::GetTouchPositionWS();
            touchParticles->Burst(15);
        }
    }
}
