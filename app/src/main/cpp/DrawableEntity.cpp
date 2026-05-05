#include "DrawableEntity.h"

DrawableEntity::DrawableEntity(RenderQueue type)
:renderQueue(type)
{ }

DrawableEntity::~DrawableEntity() { }

void DrawableEntity::Draw() const { }
