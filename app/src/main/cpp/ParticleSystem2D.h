#pragma once
#include "ParticleSystem.h"

class ParticleSystem2D : public ParticleSystem {
  public:
    ParticleSystem2D(int maxParticles);
    ParticleSystem2D(ParticleSystem2D&&)                 = default;
    ParticleSystem2D(const ParticleSystem2D&)            = default;
    ParticleSystem2D& operator=(ParticleSystem2D&&)      = default;
    ParticleSystem2D& operator=(const ParticleSystem2D&) = default;
    ~ParticleSystem2D();

    void Draw() const override;

  private:
};
