#pragma once

enum class RenderQueue { SKY, OPAQUE, TRANSPARENT, UI };

class Scene;

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

    virtual void Draw() const;

    virtual void OnEnable();

    virtual void OnDisable();

    void SetActive(bool active);
    bool IsActive() const { return m_IsActive; }

    RenderQueue GetRenderQueue() const { return m_RenderQueue; }
    void SetRenderQueue(RenderQueue type) { m_RenderQueue = type; }

private:
    bool m_IsActive;
    RenderQueue m_RenderQueue = RenderQueue::OPAQUE;
};
