#pragma once
#include "System.h"

class Entity;
class ComponentUI;


using UIEventCallback = std::function<void(Entity*)>;

class SystemUI : public System {
private:
  
    std::map<int, UIEventCallback> m_clickCallbacks;

    bool m_mousePressed;
    bool m_mousePrevPressed;
    float m_mouseX;
    float m_mouseY;

public:
    SystemUI();
    virtual ~SystemUI();

    virtual void Update(float dt, const std::vector<Entity*>& entities) override;


    void UpdateHealthBar(const std::vector<Entity*>& entities);

  
    void ProcessUIEvents(const std::vector<Entity*>& entities);
    void HandleHoverEvents(const std::vector<Entity*>& entities);
    void HandleClickEvents(const std::vector<Entity*>& entities);

    void RegisterClickCallback(Entity* entity, UIEventCallback callback);
    void UnregisterClickCallback(Entity* entity);

    void SetMouseState(float x, float y, bool pressed);
};