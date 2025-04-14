#include "pch.h"
#include "SystemCamera.h"

void SystemCamera::Update(float dt, const std::vector<Entity*>& entities) {
    if (debugMode && logSystemCamera) {
        HEAD("CLEAN") << LINE << ENDL2;
    }

    for (auto* entity : entities) {
        ComponentCamera* cameraComponent = entity->GetComponent<ComponentCamera>();
        if (!cameraComponent) continue;

        cameraComponent->SyncWithTransform();

        if (debugMode && logSystemCamera) {
            cameraComponent->Logs();
        }
    }
}