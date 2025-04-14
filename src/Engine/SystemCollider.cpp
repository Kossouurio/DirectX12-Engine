#include "pch.h"
#include "SystemCollider.h"

void SystemCollider::Update(float dt, const std::vector<Entity*>& entities) {
    if (debugMode && logSystemCollider) {
        HEAD("CLEAN") << LINE << ENDL2;
    }

    for (size_t i = 0; i < entities.size(); ++i) {
        ProcessCollisions(entities, entities[i], i);
    }

    for (auto* entity : entities) {
        ComponentCollider* collider = entity->GetComponent<ComponentCollider>();
		if (!collider) continue;

        if (debugMode && logSystemCollider) {
            collider->Logs();
        }
    }
}

void SystemCollider::ProcessCollisions(const std::vector<Entity*>& entities, Entity* entityA, size_t index) {
    ComponentCollider* colliderA = entityA->GetComponent<ComponentCollider>();
    ComponentTransform* transformA = entityA->GetComponent<ComponentTransform>();

    for (size_t j = index + 1; j < entities.size(); ++j) {
        Entity* entityB = entities[j];
        ComponentCollider* colliderB = entityB->GetComponent<ComponentCollider>();
        ComponentTransform* transformB = entityB->GetComponent<ComponentTransform>();

        if (colliderA->CheckCollision(colliderB)) {
            HandleCollision(entityA, entityB, transformA, transformB);
        }
    }
}

void SystemCollider::HandleCollision(Entity* entityA, Entity* entityB,
    ComponentTransform* transformA,
    ComponentTransform* transformB) {

    if (!transformA || !transformB) return;
    ApplyCollisionResponse(transformA, transformB, {
        transformA->GetPosition().x - transformB->GetPosition().x,
        transformA->GetPosition().y - transformB->GetPosition().y,
        transformA->GetPosition().z - transformB->GetPosition().z
        });
}

void SystemCollider::ApplyCollisionResponse(ComponentTransform* transformA,
    ComponentTransform* transformB,
    const XMFLOAT3& direction) {
    float correctionFactor = 0.1f;
    XMFLOAT3 correction = { direction.x * correctionFactor,
                            direction.y * correctionFactor,
                            direction.z * correctionFactor };

    transformA->MoveBy(correction);
    correction.x = -correction.x;
    correction.y = -correction.y;
    correction.z = -correction.z;
    transformB->MoveBy(correction);
}

Entity* SystemCollider::Raycast(XMFLOAT3 origin, XMFLOAT3 direction, float maxDistance, const std::vector<Entity*>& entities) {
    Entity* closestEntity = nullptr;
    float closestDistance = maxDistance;

    for (auto* entity : entities) {
        ComponentCollider* collider = entity->GetComponent<ComponentCollider>();
        ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
        if (!collider || !transform) continue;

        XMFLOAT3 targetPos = transform->GetPosition();
        XMFLOAT3 rayToTarget = { targetPos.x - origin.x, targetPos.y - origin.y, targetPos.z - origin.z };

        // Normaliser le vecteur du rayon vers la cible
        XMVECTOR rayToTargetVec = XMLoadFloat3(&rayToTarget);
        rayToTargetVec = XMVector3Normalize(rayToTargetVec);

        XMVECTOR directionVec = XMLoadFloat3(&direction);
        directionVec = XMVector3Normalize(directionVec);

        // Verifie l'alignement entre le rayon et la cible (cos(angle) proche de 1)
        float dotProduct = XMVectorGetX(XMVector3Dot(rayToTargetVec, directionVec));

        if (dotProduct > 0.98f) {  // Seuil de tolerance pour eviter les erreurs de precision
            float distance = sqrt(rayToTarget.x * rayToTarget.x + rayToTarget.y * rayToTarget.y + rayToTarget.z * rayToTarget.z);
            if (distance < closestDistance) {
                closestDistance = distance;
                closestEntity = entity;
            }
        }
    }

    return closestEntity;
}
