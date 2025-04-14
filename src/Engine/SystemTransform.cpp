#include "pch.h"
#include "SystemTransform.h"

void SystemTransform::Update(float dt, const std::vector<Entity*>& entities) {
    if (debugMode && logSystemTransform) {
        HEAD("CLEAN") << LINE << ENDL2;
    }

    for (auto* entity : entities) {
        ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
        if (!transform) continue;

        transform->UpdateTransform(dt);

        if (debugMode && logSystemTransform) {
			transform->Logs();
        }
    }
}
