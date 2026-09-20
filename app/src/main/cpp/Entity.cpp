#include "Entity.h"
#include "Scene.h"
#include <utility>


Entity::~Entity() {
}

void Entity::Update(float dT) {
}

void Entity::Start() {
}

void Entity::SetActive(bool active) {

    m_IsActive = active;
    if (!hasStarted) return;

    if (active)
        OnEnable();
    else
        OnDisable();
}

Entity::Entity(Scene* scene)
: parentScene(scene) {
}

void Entity::OnEnable() {
}
void Entity::OnDisable() {
}
template <typename T, typename... Args>
std::unique_ptr<T> Entity::CreateChildEntity(bool active, Args&&... args) {
    auto child = parentScene->CreateEntity<T>(active, std::forward<Args>(args)...);
    if (child.get()) {
        child->parentEntity = this;
        childEntities.push_back(child.get());
    }
    return std::move(child);
}
