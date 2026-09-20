#pragma once
#include "Entity.h"
#include "ParticleSystem.h"
#include "raylib.h"

class ElementBubbleParticles : public ParticleSystem {
  public:
    ElementBubbleParticles(Scene* parentScene);
    ~ElementBubbleParticles();

    void SetStartEndColor(Color color) {
        particleProperties.startColor = color;
        particleProperties.endColor = color;
    }
};
