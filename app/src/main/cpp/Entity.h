#pragma once

enum RenderMode { OPAQUE, ALPHA, ADDITIVE };

class Entity {
public:
    Entity();
    Entity(Entity&&) = default;
    Entity(const Entity&) = default;
    Entity& operator=(Entity&&) = default;
    Entity& operator=(const Entity&) = default;
    ~Entity();

    // protected:
    virtual void Start();

    virtual void Update(float dT = 1.0f);

    void SetActive(bool active);
    bool IsActive() const { return m_IsActive; }
    RenderMode GetRenderMode() const { return m_RenderMode; }
    void SetRenderMode(RenderMode mode) { m_RenderMode = mode; }

    virtual void Draw() const;

private:
    bool m_IsActive;
    RenderMode m_RenderMode = OPAQUE;
};
