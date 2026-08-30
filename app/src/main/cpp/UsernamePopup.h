#pragma once
#include "uiButton.h"
#include "uiElement.h"
#include "uiImage.h"
#include "uiText.h"
#include <memory>


#define MAX_USERNAME_SIZE 15
class UsernamePopup : public ui::Canvas{
  public:
    UsernamePopup(Scene* parentScene);
    ~UsernamePopup();

    std::unique_ptr<ui::Image> darkBgImg     = nullptr;
    std::unique_ptr<ui::Image> usernameBgImg = nullptr;
    std::unique_ptr<ui::Button> confirmBtn   = nullptr;
    std::unique_ptr<ui::Button> cancelBtn    = nullptr;
    std::unique_ptr<ui::Text> msgTxt         = nullptr;
    std::unique_ptr<ui::Text> usernameTxt    = nullptr;

    void OnEnable() override;
    void OnDisable() override;
    void Update(float dT = 1.0f) override;

  private:
    char usernameBuffer[MAX_USERNAME_SIZE + 1] = "\0";
};
