#pragma once

enum class RenderMode { OPAQUE, ALPHA, ADDITIVE };

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

    virtual void Draw() const;

    virtual void LoadResources();

    void SetActive(bool active);
    bool IsActive() const { return m_IsActive; }
    RenderMode GetRenderMode() const { return m_RenderMode; }
    void SetRenderMode(RenderMode mode) { m_RenderMode = mode; }


private:
    bool m_IsActive;
    RenderMode m_RenderMode = RenderMode::OPAQUE;
};
