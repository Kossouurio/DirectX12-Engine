#include "pch.h"
#include "SystemTransform.h"

void SystemTransform::AddEntity(Entity* entity) {
    if (entity->GetComponent<ComponentTransform>()) {
        entities.push_back(entity);
    }
}

void SystemTransform::Update(float dt) {
    for (auto* entity : entities) {
        ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
        if (transform) {
            transform->vPosition.x += transform->vVelocity.x * dt;
            transform->vPosition.y += transform->vVelocity.y * dt;
            transform->vPosition.z += transform->vVelocity.z * dt;

            transform->UpdateMatrix();

            HEAD("SYSTEM_TRANSFORM") << "Entity " << entity->GetID()
                << " Position: (" << transform->vPosition.x << ", "
                << transform->vPosition.y << ", "
                << transform->vPosition.z << ")" << ENDL;
        }
    }
}
