#include "BubbleTrails.h"
#include "ElementType.h"
#include "ResourceManager.h"
#include "bubbleManager.h"
#include "raylib.h"
#include <raymath.h>

BubbleTrails::BubbleTrails(Scene* parentScene)
: DrawableEntity(parentScene, RenderQueue::TRANSPARENT) {
    trailMesh = ResourceManager::GetModel("models/BubbleTrail.glb");
    trailMesh->materials[0].shader = *ResourceManager::Get().GetShader("shaders/particles.vert", "shaders/particles.frag");
    trailMesh->materials[0].maps[0].texture =
        *ResourceManager::Get().GetTexture("textures/AnemoTrailSpritesheet.png");
    tfrcId = GetShaderLocation(trailMesh->materials[0].shader, "_tfrc");
}

BubbleTrails::~BubbleTrails() {
}

void BubbleTrails::Draw() {
    tfrc.x = GetTime();
    SetShaderValue(trailMesh->materials[0].shader, tfrcId, &tfrc, SHADER_UNIFORM_VEC4);
    for (int i = 0; i < BubbleManager::Get().activeBubbleCount; i++) {
        if (BubbleManager::Get().activeBubbles[i] != nullptr) {
            Bubble* bubble = BubbleManager::Get().activeBubbles[i];
            if (bubble->type != ElementType::NONE) {
                // Trail
                DrawModelEx(*trailMesh,
                    bubble->position,
                    { 0.f, 1.f, 0.f },
                    bubble->angle,
                    Vector3One() * bubble->radius,
                    BubbleManager::Get().bubbleColors[(int)bubble->type]);
                // ColorBrightness(BubbleManager::Get().bubbleColors[(int)bubble->type], 0.5f));
            }
        }
    }
}
