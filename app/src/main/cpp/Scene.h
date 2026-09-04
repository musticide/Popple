#pragma once

#include "DrawableEntity.h"
#include "Entity.h"
#include "Log.h"
#include "ParticleSystem.h"
#include <exception>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

enum class SceneType {
    HOME,
    GAMEPLAY,
    LOADING
};

class Scene {
  public:
    // Scene(const char* name);
    Scene(SceneType type);
    Scene(Scene&&)                 = default;
    Scene(const Scene&)            = default;
    Scene& operator=(Scene&&)      = default;
    Scene& operator=(const Scene&) = default;
    virtual ~Scene();

    template <typename T, typename... Args>
    std::unique_ptr<T> CreateEntity(bool active, Args&&... args) {
        static_assert(std::is_base_of<Entity, T>::value, "T must derive from Entity");

        LOGV("%s is creating entity", m_Name);
        auto entity = std::make_unique<T>(this, std::forward<Args>(args)...);

        // entity->parentScene = this;
        entity->SetActive(active);

        LOGV("Entity added to %s", m_Name);
        m_SceneEntities.push_back(entity.get());

        if (DrawableEntity* d = entity->asDrawable()) {
            LOGV("Entity is Drawable");
            switch (d->GetRenderQueue()) {
                case RenderQueue::SKY:
                    m_Buckets.sky.push_back(d);
                    break;
                case RenderQueue::OPAQUE:
                    m_Buckets.opaque.push_back(d);
                    break;
                case RenderQueue::TRANSPARENT:
                    // if (dynamic_cast<ParticleSystem*>(d))
                    //     LOGI("Particle System added");
                    m_Buckets.transparent.push_back(d);
                    break;
                case RenderQueue::UI:
                    m_Buckets.ui.push_back(d);
                    break;
            }
        }

        return std::move(entity);
    }

    SceneType GetType() const {
        return m_Type;
    }

    bool IsActive() const {
        return m_IsActive;
    }
    void SetActive(bool active) {
        m_IsActive = active;
        if (active) {
            LOGI("Scene activated: %s", m_Name);
            for (auto e : m_SceneEntities) {
                if (e != nullptr && e->IsActive() && e->hasStarted) {
                    e->OnEnable();
                }
            }

        } else {
            LOGI("Scene deactivated: %s", m_Name);
            for (auto e : m_SceneEntities) {
                if (e != nullptr && e->IsActive() && e->hasStarted) {
                    e->OnDisable();
                }
            }
        }
    }

    virtual void Load() = 0;
    float GetProgress() const {
        return static_cast<float>(m_LoadStep) / m_TotalSteps;
    }
    void Start();
    void Update(float dT);
    void DrawSky() const;
    void DrawOpaque() const;
    void DrawTransparent() const;
    void DrawUI() const;

    bool isLoaded = false;
    bool showLoadingScreen = false;
  protected:
    const char* m_Name = "scene";
    SceneType m_Type;
    std::vector<Entity*> m_SceneEntities;

    struct RenderBuckets {
        std::vector<DrawableEntity*> sky;
        std::vector<DrawableEntity*> opaque;
        std::vector<DrawableEntity*> transparent;
        std::vector<DrawableEntity*> ui;
    } m_Buckets;

    int m_LoadStep = 0, m_TotalSteps = 1;

  private:
    bool m_IsActive;
};
