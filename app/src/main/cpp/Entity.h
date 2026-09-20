#pragma once

#include <memory>
#include <vector>
class Scene;

class DrawableEntity;

class Entity {
public:
    Entity(Scene* scene);
    Entity(Entity&&) = default;
    Entity(const Entity&) = default;
    Entity& operator=(Entity&&) = default;
    Entity& operator=(const Entity&) = default;
    virtual ~Entity();

    Scene* parentScene;
    bool hasStarted = false;
    Entity* parentEntity;

    // protected:
    virtual void Start();

    virtual void Update(float dT = 1.0f);

    virtual void OnEnable();

    virtual void OnDisable();

    virtual DrawableEntity* asDrawable() { return nullptr; }

    void SetActive(bool active);
    bool IsActive() const { return m_IsActive; }


    bool m_IsActive;
    template <typename T, typename... Args>
    std::unique_ptr<T> CreateChildEntity(bool active, Args&&... args);

  private:
    std::vector<Entity*> childEntities;
};

