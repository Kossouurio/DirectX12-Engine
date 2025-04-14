#include "pch.h"
#include "ComponentScore.h"


ComponentUI::ComponentUI(Entity* _pEntity)
    : Component(_pEntity),
    m_type(UIElementType::BUTTON),
    m_text(""),
    m_color(XMFLOAT4(0.2f, 0.2f, 0.8f, 1.0f)),         // Bleu par defaut
    m_hoverColor(XMFLOAT4(0.3f, 0.3f, 0.9f, 1.0f)),    // Bleu plus clair en survol
    m_textColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)),
    m_fillColor(0.0f,0.0f,0.0f,0.0f),
    m_isClickable(false),
    m_isVisible(true),
    m_isHovered(false),
    m_isPressed(false),
    m_width(100.0f),
    m_height(50.0f),
    m_linkedHealth(nullptr),
    m_linkedScore(nullptr)
{
}

ComponentUI::~ComponentUI()
{
}

bool ComponentUI::ContainsPoint(float x, float y)
{
    
    ComponentTransform* transform = GetEntity()->GetComponent<ComponentTransform>();
    if (!transform) return false;

    XMFLOAT3 position = transform->GetPosition();

    return (x >= position.x && x <= position.x + m_width &&
        y >= position.y && y <= position.y + m_height);
}

void ComponentUI::LinkToHealth(ComponentHealth* health) {
    m_linkedHealth = health;
    if (health && m_type == UIElementType::LIFE) {
        health->SetHealth(health->GetHealth());
        health->SetMaxHealth(health->GetMaxHealth());
    }
}

void ComponentUI::LinkToScore(ComponentScore* score) {
    m_linkedScore = score;
    if (score && m_type == UIElementType::SCORE) {
        score->SetScore(score->GetScore());
        SetText("Score: " + std::to_string(score->GetScore()));
    }
}

void ComponentUI::Update() {
    if (m_type == UIElementType::LIFE && m_linkedHealth) {
        
        m_linkedHealth->SetHealth(m_linkedHealth->GetHealth());
        m_linkedHealth->SetMaxHealth(m_linkedHealth->GetMaxHealth());

       
        float healthRatio = (float)m_linkedHealth->GetHealth() / m_linkedHealth->GetMaxHealth();
        if (healthRatio > 0.6f) {
            SetFillColor(XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f)); 
        }
        else if (healthRatio > 0.3f) {
            SetFillColor(XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f)); 
        }
        else {
            SetFillColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)); 
        }
    }
    else if (m_type == UIElementType::SCORE && m_linkedScore) {
        m_linkedScore->SetScore(m_linkedScore->GetScore());
        SetText("Score: " + std::to_string(m_linkedScore->GetScore()));
    }
}

void ComponentUI::Logs()
{
    HEAD("UI COMPONENT") << "ID: " << GetID()
        << " | Type: " << static_cast<int>(m_type)
        << " | Text: " 
        << " | Visible: " << (m_isVisible ? "Yes" : "No")
        << " | Clickable: " << (m_isClickable ? "Yes" : "No")
        << " | Size: " << m_width << "x" << m_height
        << ENDL;
}