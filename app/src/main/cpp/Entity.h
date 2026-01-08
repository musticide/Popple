#pragma once

class Scene;

class DrawableEntity;

class Entity {
public:
    Entity();
    Entity(Scene* scene);
    Entity(Entity&&) = default;
    Entity(const Entity&) = default;
    Entity& operator=(Entity&&) = default;
    Entity& operator=(const Entity&) = default;
    ~Entity();

    Scene* parentScene;
    bool hasStarted = false;

    // protected:
    virtual void Start();

    virtual void Update(float dT = 1.0f);

    virtual void OnEnable();

    virtual void OnDisable();

    virtual DrawableEntity* asDrawable() { return nullptr; }

    void SetActive(bool active);
    bool IsActive() const { return m_IsActive; }


private:
    bool m_IsActive;
};
