#include "pch.h"
#include "SystemRender.h"

void SystemRender::AddEntity(Entity* entity) {
    if (entity->GetComponent<ComponentRender>()) {
        entities.push_back(entity);
    }
}

void SystemRender::Update(float dt) {
    for (auto* entity : entities) {
        ComponentRender* render = entity->GetComponent<ComponentRender>();
        if (render && render->visible) {
            std::cout << "[Render] Entity ID: " << entity->GetID() << " | Color: "
                << render->color.x << ", " << render->color.y << ", " << render->color.z << "\n";
        }
    }
}
