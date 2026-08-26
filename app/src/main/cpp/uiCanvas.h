#pragma once
#include "DrawableEntity.h"
#include "Log.h"
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace ui {
class UIElement;
class Canvas : public DrawableEntity {
  public:
    Canvas(Scene* parentScene);
    Canvas(Canvas&&)                 = default;
    Canvas(const Canvas&)            = default;
    Canvas& operator=(Canvas&&)      = default;
    Canvas& operator=(const Canvas&) = default;
    virtual ~Canvas();

    // WARN:
    Scene* parentScene;

    template <typename T, typename... Args>
    std::unique_ptr<T> CreateElement(bool active, Args&&... args) {
        static_assert(std::is_base_of<UIElement, T>::value, "T must derive from UIElement");

        auto element = std::make_unique<T>(this->parentScene, (Canvas*)this, std::forward<Args>(args)...);

        element->SetActive(active);

        if (element.get())
            m_Elements.push_back(element.get());
        else
            LOGE("Failed to create element");

        return std::move(element);
    }

    void Start() override;

    void Update(float dT = 1.0f) override;

    void Draw() const override;

  private:
    std::vector<UIElement*> m_Elements;
};
} // namespace ui
