#include "DrawableEntity.h"

DrawableEntity::DrawableEntity(Scene* parentScene, RenderQueue type)
:Entity(parentScene), renderQueue(type)
{ }

DrawableEntity::~DrawableEntity() { }

void DrawableEntity::Draw() const { }
