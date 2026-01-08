#include "DrawableEntity.h"

DrawableEntity::DrawableEntity(RenderQueue type)
:m_RenderQueue(type)
{ }

DrawableEntity::~DrawableEntity() { }

void DrawableEntity::Draw() const { }
