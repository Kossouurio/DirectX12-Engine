#include "pch.h"
  

SystemUI::SystemUI()
    : m_mousePressed(false),
    m_mousePrevPressed(false),
    m_mouseX(0.0f),
    m_mouseY(0.0f)
{
}

SystemUI::~SystemUI()
{
    m_clickCallbacks.clear();
}

void SystemUI::Update(float dt, const std::vector<Entity*>& entities) {
   

   
    int uiCount = 0;
    for (auto* entity : entities) {
        if (entity->GetComponent<ComponentUI>()) {
            uiCount++;
        }
    }
    
    m_mousePrevPressed = m_mousePressed;
    m_mousePressed = Input::IsKeyPressed(VK_LBUTTON);

    UpdateHealthBar(entities);
    ProcessUIEvents(entities);
}

void SystemUI::UpdateHealthBar(const std::vector<Entity*>& entities) {
    for (auto* entity : entities) {
        ComponentUI* ui = entity->GetComponent<ComponentUI>();
        if (!ui || ui->GetType() != UIElementType::LIFE) continue;
        
        ComponentHealth* health = ui->GetLinkedHealth();
        if (!health) continue;
        
        ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
        if (!transform) continue;
        
        ComponentMesh* mesh = entity->GetComponent<ComponentMesh>();
        if (!mesh) continue;
        
     
        float healthRatio = static_cast<float>(health->GetHealth()) / health->GetMaxHealth();
        
    
        float baseWidth = 0.4f;
        float height = 0.05f;
        float depth = 0.01f;
        
   
        transform->SetScale(XMFLOAT3(baseWidth * healthRatio, height, depth));
        
       
        if (healthRatio > 0.6f) {
            mesh->GetMesh()->SetColor(0.0f, 0.8f, 0.0f); 
        } else if (healthRatio > 0.3f) {
            mesh->GetMesh()->SetColor(1.0f, 0.5f, 0.0f); 
        } else {
            mesh->GetMesh()->SetColor(1.0f, 0.0f, 0.0f); 
        }
    }
}

void SystemUI::ProcessUIEvents(const std::vector<Entity*>& entities)
{
   
    HandleHoverEvents(entities);

    
    HandleClickEvents(entities);
}

void SystemUI::HandleHoverEvents(const std::vector<Entity*>& entities)
{
    for (Entity* entity : entities)
    {
        ComponentUI* ui = entity->GetComponent<ComponentUI>();
        if (!ui || !ui->IsVisible()) continue;

       
        bool isHovered = ui->ContainsPoint(m_mouseX, m_mouseY);
        ui->SetHovered(isHovered);
    }
}

void SystemUI::HandleClickEvents(const std::vector<Entity*>& entities)
{
   
    bool mouseClicked = m_mousePrevPressed && !m_mousePressed;

    for (Entity* entity : entities)
    {
        ComponentUI* ui = entity->GetComponent<ComponentUI>();
        if (!ui || !ui->IsVisible() || !ui->IsClickable()) continue;

      
        if (ui->IsHovered() && m_mousePressed)
        {
            ui->SetPressed(true);
        }
        else if (ui->IsPressed() && !m_mousePressed)
        {
            ui->SetPressed(false);

            
            if (ui->IsHovered() && mouseClicked)
            {
               
                int entityId = entity->GetID();
                if (m_clickCallbacks.find(entityId) != m_clickCallbacks.end())
                {
                    m_clickCallbacks[entityId](entity);
                }

                HEAD("UI SYSTEM") << "Button clicked: " << ENDL;
            }
        }
    }
}

void SystemUI::RegisterClickCallback(Entity* entity, UIEventCallback callback)
{
    if (entity)
    {
        m_clickCallbacks[entity->GetID()] = callback;
    }
}

void SystemUI::UnregisterClickCallback(Entity* entity)
{
    if (entity)
    {
        m_clickCallbacks.erase(entity->GetID());
    }
}

void SystemUI::SetMouseState(float x, float y, bool pressed)
{
    m_mousePrevPressed = m_mousePressed;
    m_mousePressed = pressed;
    m_mouseX = x;
    m_mouseY = y;
}

