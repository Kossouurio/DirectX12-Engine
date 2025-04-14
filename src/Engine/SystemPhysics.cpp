#include "pch.h"
#include "SystemPhysics.h"

void SystemPhysics::Update(float dt, const std::vector<Entity*>& entities) {
    if (debugMode && logSystemPhysics) {
        HEAD("CLEAN") << LINE << ENDL2;
    }

    for (auto* entity : entities) {
        ComponentPhysics* physics = entity->GetComponent<ComponentPhysics>();
		if (!physics) continue;

        physics->UpdatePhysics(dt);

        if (debugMode && logSystemPhysics) {
            physics->Logs();
        }

        if (!physics->IsAffectedByGravity()) continue;

        physics->ApplyPhysics(dt);

        
    }
}

