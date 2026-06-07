#include "ParticleSystem2D.h"
#include "ParticleSystem.h"

ParticleSystem2D::ParticleSystem2D(int maxParticles) : ParticleSystem(maxParticles) {
    renderQueue = RenderQueue::UI;
}

ParticleSystem2D::~ParticleSystem2D() {
}

void ParticleSystem2D::Draw() const {
}
