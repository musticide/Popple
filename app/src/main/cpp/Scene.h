#pragma once

#include "Entity.h"
#include "Log.h"
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

class Scene {
public:
    Scene(const char* name);
    Scene(Scene&&) = default;
    Scene(const Scene&) = default;
    Scene& operator=(Scene&&) = default;
    Scene& operator=(const Scene&) = default;
    ~Scene();

    template <typename T, typename... Args> T* CreateEntity(Args&&... args)
    {
        static_assert(std::is_base_of<Entity, T>::value, "T must derive from value");

        auto entity = std::make_unique<T>(std::forward<Args>(args)...);
        entity->parentScene = this;

        T* ptr = entity.get();
        AddEntity(std::move(entity));
        return ptr;
    }

    const char* const GetName() const { return m_Name;}

    bool IsActive() const { return m_IsActive; }
    void SetActive(bool active) { m_IsActive = active; }

    bool HasLoaded() const { return m_Loaded; }
    bool HasStarted() const { return m_HasStarted; }

    void AddEntity(std::unique_ptr<Entity> entity);

    void Load();
    void Start();
    void Update(float dT);
    void Draw();
    void DrawUI();

private:
    const char* m_Name;
    std::vector<std::unique_ptr<Entity>> m_SceneEntities;
    bool m_IsActive;
    bool m_Loaded = false;
    bool m_HasStarted = false;

    struct RenderBuckets {
        std::vector<Entity*> sky;
        std::vector<Entity*> opaque;
        std::vector<Entity*> transparent;
        std::vector<Entity*> ui;
    } m_Buckets;
};
