#pragma once

#include "Entity.h"

enum class RenderQueue { SKY, OPAQUE, TRANSPARENT, UI };

class DrawableEntity : public Entity {
public:
    DrawableEntity(RenderQueue type);
    DrawableEntity(DrawableEntity&&) = default;
    DrawableEntity(const DrawableEntity&) = default;
    DrawableEntity& operator=(DrawableEntity&&) = default;
    DrawableEntity& operator=(const DrawableEntity&) = default;
    ~DrawableEntity();

    RenderQueue GetRenderQueue() const { return m_RenderQueue; }

    DrawableEntity* asDrawable() override { return this; }

    virtual void Draw() const;

private:
    RenderQueue m_RenderQueue = RenderQueue::OPAQUE;
};
