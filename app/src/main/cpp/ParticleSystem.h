#pragma once

#include "DrawableEntity.h"
#include "Entity.h"
#include "Tween.h"
#include "raylib.h"
#include "raymath.h"
#include <functional>
#include <memory>
#include <sys/types.h>
#include <vector>

struct ParticleProperties {
    // Particle
    float lifetime = 1.0f;

    float initialSpeed   = 0.0f;
    float speedVariation = 0.0f;
    /// Higher value is more damping
    float damping = 0.1f;

    float startSize     = 1.0f;
    float endSize       = 1.0f;
    float sizeVariation = 0.0f;

    // Rendering
    Color startColor = WHITE;
    Color endColor   = WHITE;
};

struct Particle {
    bool isActive  = false;
    float lifetime = 1.0f;
    float age      = 0.0f;

    Vector3 velocity = (Vector3){ 0, 0, 0 };
    Vector3 position = (Vector3){ 0, 0, 0 };
    float size       = 1.0f;
    float rotation   = 0.0f;
    Vector2 data     = (Vector2){ 0, 0 };

    Color color = WHITE;
};

struct TextureProperties {
    bool isSpriteSheet = false;
    int rows           = 1;
    int columns        = 1;
    int fps            = 4;


    Vector4& GetTfrc() {
        tfrc.x = GetTime();
        tfrc.y = fps;
        tfrc.z = rows;
        tfrc.w = columns;

        return tfrc;
    }
    int timeFpsRowsColumsId = -1;

  private:
    Vector4 tfrc = Vector4Zero();
};

enum class EmitShape {
    NONE,
    CIRCLE,
    LINE,
    CUSTOM
};

enum class EmitType {
    CONTINUOUS,
    BURST
};

class ParticleSystem : public DrawableEntity {
  public:
    ParticleSystem(Scene* parentScene, int maxParticles);
    ~ParticleSystem();

    ParticleProperties particleProperties;
    Vector3 position = { 0, 0, 0 };
    bool localSpace = false;
    Vector3* localPosition = nullptr;



    /// Default Continuous
    EmitType emitType = EmitType::CONTINUOUS;
    /// Rate of emission per second
    int emitRate     = 2;
    int maxParticles = 50;

    EmitShape shape;
    /// Used when shape NONE
    Vector3 direction = Vector3Zero();

    int circleRadius         = 0;
    int circleRadiusInternal = 0;

    /// Used when shape LINE
    Vector3 endPoint    = Vector3Zero();
    float endPointForce = 0.1f;

    Easing::Func easeFunc = Easing::Linear;

    // std::shared_ptr<Model> model;
    Model model;
    /// uses default material if not specified
    Material material;

    void Start() override;
    void Update(float dT = 1.0f) override;
    void Draw() override;

    void Burst(int amount);

    std::function<Vector3()> customShapeFunc;

    bool isSimulating = false;

    TextureProperties textureProperties;

  private:
    std::vector<Particle> m_ParticlePool;
    int m_CurrentIndex = 0;
    float m_TimeElapsed = 0;
    float particleIndexId = -1;

    Vector3 GetCircularDirection() {
        float randAngle = GetRandomValue(0, 360);
        return (Vector3){ (float)cos(randAngle), 0, (float)sin(randAngle) };
    }
    void Emit();
};
