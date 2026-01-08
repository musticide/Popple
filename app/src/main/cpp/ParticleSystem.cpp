#include "ParticleSystem.h"
#include "DrawableEntity.h"
#include "Log.h"
#include "ResourceManager.h"
#include "functionLibrary.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

ParticleSystem::ParticleSystem(int maxParticles)
    : DrawableEntity(RenderQueue::TRANSPARENT)
    , maxParticles(maxParticles)
{
    m_CurrentIndex = maxParticles - 1;
}
ParticleSystem::~ParticleSystem() { }
void ParticleSystem::Start()
{
    // if (model == nullptr)
    // model = LoadModel("models/Quad.glb");
    model = LoadModelFromMesh(ResourceManager::GetModel("models/Quad.glb")->meshes[0]);
    model.materials[0].shader = *ResourceManager::GetShader("shaders/basicShader.vs", "shaders/basicShader.fs");
    model.materials[0].maps[0].texture = *ResourceManager::GetTexture("textures/GlowTight.png");

    // m_ParticlePool.resize(maxParticles);
    for (size_t i = 0; i < maxParticles; i++) {
        m_ParticlePool.emplace_back();
    }
    LOGI("particle pool size: %zu", m_ParticlePool.size());
}

void ParticleSystem::Update(float dT)
{
    // emit particles
    if (emitType == EmitType::CONTINUOUS) {
        m_TimeElapsed += dT;
        float emitTime = m_TimeElapsed - (1.0f / emitRate);
        if (emitTime >= 0) {
            int emitAmount = std::floor(emitTime * emitRate) + 1;
            for (size_t i = 0; i < emitAmount; i++) {
                Emit();
            }
            m_TimeElapsed = 0.0f;
        }
    }

    int activeParticleCount = 0;

    // update particles
    for (size_t i = 0; i < m_ParticlePool.size(); i++) {
        Particle& particle = m_ParticlePool[i];

        if (!particle.isActive)
            continue;

        // activeParticleCount++;
        particle.age += dT;

        if (particle.age >= particle.lifetime) {
            particle.isActive = false;
            // activeParticleCount--;
            continue;
        }
        float life = particle.age / particle.lifetime;
        particle.color = ColorLerp(particleProperties.startColor, particleProperties.endColor, life);
        particle.size = Lerp(particleProperties.size, 0.0f, life);
        particle.velocity *= 0.95f;
        particle.position += particle.velocity;
        // TODO: Transform paricles for local space emission
    }
    // if (activeParticleCount != 0)
    //     LOGI("Active Particle Count: %d", activeParticleCount);
}

void ParticleSystem::Draw() const
{
    for (const Particle& particle : m_ParticlePool) {
        if (particle.isActive) {
            // LOGI("DrawingParticle");
            DrawModel(model, particle.position, particle.size, particle.color);
        }
    }
}

void ParticleSystem::Emit()
{
    // LOGI("Particle Emitted");
    Particle& particle = m_ParticlePool[m_CurrentIndex];
    particle.isActive = true;
    particle.lifetime = particleProperties.lifetime;
    particle.age = 0.0f;
    particle.color = particleProperties.startColor;
    particle.position = this->position;

    particle.size = particleProperties.size
        + RandomRangeFloat(-particleProperties.sizeVariation, particleProperties.sizeVariation);

    switch (shape) {
    case EmitShape::NONE:
        particle.velocity = direction * particleProperties.initialSpeed;
    case EmitShape::CIRCLE:
        particle.velocity = GetCircularDirection() * particleProperties.initialSpeed;
        break;
    }

    m_CurrentIndex = (--m_CurrentIndex + m_ParticlePool.size()) % m_ParticlePool.size();
}
void ParticleSystem::Burst(int amount)
{
    int count = 0;
    while (count < amount) {
        Emit();
        count++;
    }
}
