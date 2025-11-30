#include "Entity.h"

Entity::Entity() { }

Entity::~Entity() { }

void Entity::Update(float dT) { }

void Entity::Start() { }

void Entity::Draw() const { }

void Entity::SetActive(bool active) { m_IsActive = active; }

