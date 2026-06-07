#pragma once

#include "Entity.h"

enum class RenderQueue { SKY, OPAQUE, TRANSPARENT, UI };
class Scene;

class DrawableEntity : public Entity {
public:
    DrawableEntity(Scene* parentScene, RenderQueue type);
    DrawableEntity(DrawableEntity&&) = default;
    DrawableEntity(const DrawableEntity&) = default;
    DrawableEntity& operator=(DrawableEntity&&) = default;
    DrawableEntity& operator=(const DrawableEntity&) = default;
    ~DrawableEntity();

    RenderQueue GetRenderQueue() const { return renderQueue; }

    DrawableEntity* asDrawable() override { return this; }

    virtual void Draw() const;

    RenderQueue renderQueue = RenderQueue::OPAQUE;
private:
};
