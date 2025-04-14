#pragma once
#include "Component.h"

class ComponentHealth;
class ComponentScore;

enum class UIElementType {
    BUTTON,
    LABEL,
    PANEL,
    LIFE,
    SCORE
};

class ComponentUI : public Component {
private:
    UIElementType m_type;
    std::string m_text;
    XMFLOAT4 m_color;         
    XMFLOAT4 m_hoverColor;    
    XMFLOAT4 m_textColor;
    bool m_isClickable;
    bool m_isVisible;
    bool m_isHovered;
    bool m_isPressed;
    XMFLOAT4 m_fillColor;

    float m_width;
    float m_height;

    ComponentHealth* m_linkedHealth;
    ComponentScore* m_linkedScore;

public:
    ComponentUI(Entity* _pEntity);
    virtual ~ComponentUI();

    void Update();

    // Setters
    void SetType(UIElementType type) { m_type = type; }
    void SetText(const std::string& text) { m_text = text; }
    void SetClickable(bool clickable) { m_isClickable = clickable; }
    void SetVisible(bool visible) { m_isVisible = visible; }
    void SetDimensions(float width, float height) { m_width = width; m_height = height; }
    void SetFillColor(const XMFLOAT4& color) { m_fillColor = color; }
   
  

    // Getters
    const XMFLOAT4& GetFillColor() const { return m_fillColor; }
    UIElementType GetType() const { return m_type; }
    const std::string& GetText() const { return m_text; }
    bool IsClickable() const { return m_isClickable; }
    bool IsVisible() const { return m_isVisible; }
    bool IsHovered() const { return m_isHovered; }
    bool IsPressed() const { return m_isPressed; }
    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }
    ComponentHealth* GetLinkedHealth() const { return m_linkedHealth; }
    ComponentScore* GetLinkedScore() const { return m_linkedScore; }
    
    void SetHovered(bool hovered) { m_isHovered = hovered; }
    void SetPressed(bool pressed) { m_isPressed = pressed; }

    bool ContainsPoint(float x, float y);
    void LinkToHealth(ComponentHealth* health);
    void LinkToScore(ComponentScore* score);

    virtual void Logs() override;
};