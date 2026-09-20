#include "ParticleSystem.h"
#include "DrawableEntity.h"
#include "Log.h"
#include "ResourceManager.h"
#include "functionLibrary.h"
#include "raylib.h"
#include "raymath.h"
#include "utils.h"
#include <cmath>

ParticleSystem::ParticleSystem(Scene* parentScene, int maxParticles)
: DrawableEntity(parentScene, RenderQueue::TRANSPARENT)
, maxParticles(maxParticles) {
    m_CurrentIndex = maxParticles - 1;
    model          = LoadModelFromMesh(ResourceManager::GetModel("models/Quad.glb")->meshes[0]);
    model.materials[0].shader = *ResourceManager::GetShader("shaders/particles.vert", "shaders/particles.frag");
    model.materials[0].maps[0].texture = *ResourceManager::GetTexture("textures/GlowTight.png");
    // model.materials[0].params[0].

    // m_ParticlePool.resize(maxParticles);
    for (size_t i = 0; i < maxParticles; i++) {
        m_ParticlePool.emplace_back();
        m_ParticlePool[i].data.x = i;
    }
    // LOGI("particle pool size: %zu", m_ParticlePool.size());

    textureProperties.timeFpsRowsColumsId = GetShaderLocation(model.materials[0].shader, "_tfrc");
}
ParticleSystem::~ParticleSystem() {
}
void ParticleSystem::Start() {
    particleIndexId = GetShaderLocation(model.materials[0].shader, "_ParticleId");
}

void ParticleSystem::Update(float dT) {
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

        if (!particle.isActive) continue;

        activeParticleCount++;
        particle.age += dT;

        if (particle.age >= particle.lifetime) {
            particle.isActive = false;
            activeParticleCount--;
            continue;
        }
        float life      = particle.age / particle.lifetime;
        float easedLife = easeFunc(life);
        particle.color  = ColorLerp(particleProperties.startColor, particleProperties.endColor, easedLife);
        particle.size   = Lerp(particleProperties.startSize, particleProperties.endSize, easedLife);

        if (shape == EmitShape::LINE)
            particle.velocity += Vector3Normalize(endPoint - particle.position) * endPointForce *
                (1.f - particleProperties.damping);

        particle.velocity *= 1.f - particleProperties.damping;
        particle.position += particle.velocity;


        float angleRad = atan2f(particle.velocity.x, particle.velocity.z);

        particle.rotation = (angleRad * RAD2DEG);

        // TODO: Transform paricles for local space emission
    }
    // model.materials[0].params[0] = &textureProperties.GetTfrc();
    isSimulating = activeParticleCount > 0;
}

void ParticleSystem::Draw() {
    // Model model = this->model;
    if (textureProperties.timeFpsRowsColumsId >= 0) {
        SetShaderValue(
            model.materials[0].shader, textureProperties.timeFpsRowsColumsId, &textureProperties.GetTfrc(), SHADER_UNIFORM_VEC4);
    }
    for (size_t i = 0; i < m_ParticlePool.size(); i++) {
        auto& particle = m_ParticlePool[i];
        if (particle.isActive) {
            Vector3 position = particle.position;
            if (localSpace && localPosition != nullptr) position += *localPosition;
            model.meshes->texcoords2 = const_cast<float*>(&particle.data.x);
            DrawModelEx(
                model, position, { 0.f, 1.f, 0.f }, particle.rotation, Vector3One() * particle.size, particle.color);
        }
    }
}

void ParticleSystem::Emit() {
    // LOGI("Particle Emitted");
    Particle& particle = m_ParticlePool[m_CurrentIndex];
    particle.isActive  = true;
    particle.lifetime  = particleProperties.lifetime;
    particle.age       = 0.0f;
    particle.color     = particleProperties.startColor;
    particle.position  = this->position;

    particle.size = particleProperties.startSize +
        RandomRangeFloat(-particleProperties.sizeVariation, particleProperties.sizeVariation);

    float speed = particleProperties.initialSpeed +
        RandomRangeFloat(-particleProperties.speedVariation, particleProperties.speedVariation);

    switch (shape) {
        case EmitShape::NONE:
            particle.velocity = direction * speed;
            break;
        case EmitShape::CIRCLE: {
            if (circleRadius > 0) {

                float distance  = GetRandomValue(circleRadiusInternal, circleRadius);
                float randAngle = GetRandomValue(0, 360);
                particle.position += (Vector3){ (float)cos(randAngle) * distance, 0, (float)sin(randAngle) * distance };
            }
            particle.velocity = GetCircularDirection() * speed;
        } break;
        case EmitShape::LINE:
            particle.velocity = GetCircularDirection() * speed;
            particle.velocity += direction * speed;
            break;
        case EmitShape::CUSTOM:
            if (customShapeFunc) customShapeFunc();
            break;
    }

    m_CurrentIndex = (--m_CurrentIndex + m_ParticlePool.size()) % m_ParticlePool.size();
}
void ParticleSystem::Burst(int amount) {
    int count = 0;
    while (count < amount) {
        Emit();
        count++;
    }
}
