#include "Entity.h"
#include "Scene.h"

Entity::Entity() { }

Entity::~Entity() { }

void Entity::Update(float dT) { }

void Entity::Start() { }

void Entity::SetActive(bool active)
{
    m_IsActive = active;
    if (active)
        OnEnable();
    else
        OnDisable();
}

Entity::Entity(Scene* scene)
    : parentScene(scene)
{
}

void Entity::OnEnable() { }
void Entity::OnDisable() { }
