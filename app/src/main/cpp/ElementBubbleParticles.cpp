#include "ElementBubbleParticles.h"
#include "ParticleSystem.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "raylib.h"
#include "utils.h"

ElementBubbleParticles::ElementBubbleParticles(Scene* parentScene)
: ParticleSystem(parentScene, 10) {

    this->particleProperties.lifetime        = 0.5f;
    this->particleProperties.startSize       = 0.1f;
    this->particleProperties.endSize         = 0.13f;
    this->particleProperties.sizeVariation   = 0.05f;
    this->emitType                           = EmitType::CONTINUOUS;
    this->emitRate                           = 5;
    this->shape                              = EmitShape::CIRCLE;
    this->circleRadius                       = 3;
    this->particleProperties.initialSpeed    = 0.1f;
    this->particleProperties.speedVariation  = 0.3f;
    this->particleProperties.damping         = 0.45f;
    this->particleProperties.startColor      = { 205, 120, 255, 255 };
    this->particleProperties.endColor        = { 205, 120, 255, 255 };
    this->model.materials[0].maps[0].texture = *ResourceManager::GetTexture("textures/sparks.png");
    this->textureProperties.fps              = 4;
    this->textureProperties.rows             = 2;
    this->textureProperties.columns          = 2;
    this->textureProperties.isSpriteSheet    = true;
    this->localSpace                         = true;
}

ElementBubbleParticles::~ElementBubbleParticles() {
}
