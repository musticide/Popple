#pragma once

class Entity {
public:
    Entity();
    Entity(Entity &&) = default;
    Entity(const Entity &) = default;
    Entity &operator=(Entity &&) = default;
    Entity &operator=(const Entity &) = default;
    ~Entity();

// protected:
    virtual void Start();

    virtual void Update(float dT = 1.0f);

    void SetActive(bool active);
    bool IsActive() const { return m_IsActive; }

    virtual void Draw() const;

private:
    bool m_IsActive;

};
