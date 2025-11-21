#include "Entity.h"
#include "EntityManager.h"

Entity::Entity() { EntityManager::AddEntity(this); }

Entity::~Entity() { EntityManager::Remove(this); }

void Entity::Update(float dT) { }

void Entity::Start() { }

void Entity::Draw() const { }

void Entity::SetActive(bool active) { m_IsActive = active; }

void Entity::LoadResources() { }
