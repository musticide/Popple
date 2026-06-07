#include "uiCanvas.h"
#include "DrawableEntity.h"
#include "uiElement.h"

using namespace ui;

Canvas::Canvas() 
:DrawableEntity(RenderQueue::UI)
{
}

Canvas::~Canvas() {
}

void ui::Canvas::Start(){
    for (size_t i = 0; i < m_Elements.size(); i++) {
        if (m_Elements[i] && m_Elements[i]->IsActive() && !m_Elements[i]->hasStarted) {
            m_Elements[i]->Start();
        }
    }

    DrawableEntity::Start();
}

void ui::Canvas::Update(float dT) {
    for (size_t i = 0; i < m_Elements.size(); i++) {
        if (m_Elements[i] && m_Elements[i]->IsActive()) {
            m_Elements[i]->Update(dT);
        }
    }
}

void ui::Canvas::Draw() const {
    for (size_t i = 0; i < m_Elements.size(); i++) {
        if (m_Elements[i] && m_Elements[i]->IsActive()) {
            m_Elements[i]->Draw();
        }
    }
}

