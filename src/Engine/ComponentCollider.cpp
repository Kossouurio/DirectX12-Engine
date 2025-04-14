#include "pch.h"
#include "ComponentCollider.h"

ComponentCollider::ComponentCollider(Entity* entity, ColliderType type, const XMFLOAT3& size, bool trigger)
    : Component(entity), colliderType(type), colliderSize(size), isTrigger(trigger) 
{
}

void ComponentCollider::Logs() {
	HEAD("sys_COLLIDER") << "Entity " << GetEntity()->GetID() << " is trigger : " << isTrigger << ENDL;
	HEAD("sys_COLLIDER") << "Entity " << GetEntity()->GetID() << " | Collider type : " << (colliderType == ColliderType::AABB ? "AABB" : "SPHERE") << ENDL;
}


void ComponentCollider::SetColliderType(ColliderType type) 
{
    colliderType = type;
}

ColliderType ComponentCollider::GetColliderType() const
{
    return colliderType;
}

void ComponentCollider::SetColliderSize(const XMFLOAT3& size) 
{
    colliderSize = size;
}

XMFLOAT3 ComponentCollider::GetColliderSize() const
{
    return colliderSize;
}

void ComponentCollider::SetActive(bool active)
{
	isActive = active;
}

void ComponentCollider::UpdateColliderSize() {
    ComponentMesh* mesh = GetEntity()->GetComponent<ComponentMesh>();
    if (mesh) {
        XMFLOAT3 newSize = mesh->GetBoundingBox();
        SetColliderSize(newSize);

        // LOG les nouvelles dimensions
        HEAD("COLLIDER") << "Mise a jour Collider - Entite: " << GetEntity()->GetID()
            << " | Taille: (" << newSize.x << ", " << newSize.y << ", " << newSize.z << ")" << ENDL;
    }
}


void ComponentCollider::SetTrigger(bool trigger) 
{
    isTrigger = trigger;
}

bool ComponentCollider::IsTrigger() const 
{
    return isTrigger;
}

bool ComponentCollider::CheckCollision(ComponentCollider* other) 
{
    if (!other || !other->isActive) return false;

    ComponentTransform* transformA = GetEntity()->GetComponent<ComponentTransform>();
    ComponentTransform* transformB = other->GetEntity()->GetComponent<ComponentTransform>();

    if (!transformA || !transformB) return false;

    XMFLOAT3 posA = transformA->GetPosition();
    XMFLOAT3 posB = transformB->GetPosition();

    if (colliderType == ColliderType::AABB && other->GetColliderType() == ColliderType::AABB)
    {
        if (debugMode && logSystemCollider) {
            HEAD("sys_COLLIDER") << "AABB vs AABB" << ENDL;
        }
        return (abs(posA.x - posB.x) * 2 < (colliderSize.x + other->colliderSize.x)) &&
            (abs(posA.y - posB.y) * 2 < (colliderSize.y + other->colliderSize.y)) &&
            (abs(posA.z - posB.z) * 2 < (colliderSize.z + other->colliderSize.z));
    }
    else if (colliderType == ColliderType::SPHERE && other->GetColliderType() == ColliderType::SPHERE)
    {
        if (debugMode && logSystemCollider) {
            HEAD("sys_COLLIDER") << "SPHERE vs SPHERE" << ENDL;
        }
        float radiusA = colliderSize.x / 2;
        float radiusB = other->colliderSize.x / 2;

        float distanceSquared = (posA.x - posB.x) * (posA.x - posB.x) +
            (posA.y - posB.y) * (posA.y - posB.y) +
            (posA.z - posB.z) * (posA.z - posB.z);

        float radiusSum = radiusA + radiusB;
        return distanceSquared <= (radiusSum * radiusSum);
	}
	else if (colliderType == ColliderType::AABB && other->GetColliderType() == ColliderType::SPHERE)
	{
        if (debugMode && logSystemCollider) {
            HEAD("sys_COLLIDER") << "AABB vs SPHERE" << ENDL;
        }
		float radius = other->colliderSize.x / 2;

		float x = max(posA.x - colliderSize.x / 2, min(posB.x, posA.x + colliderSize.x / 2));
		float y = max(posA.y - colliderSize.y / 2, min(posB.y, posA.y + colliderSize.y / 2));
		float z = max(posA.z - colliderSize.z / 2, min(posB.z, posA.z + colliderSize.z / 2));

		float distance = sqrt((x - posB.x) * (x - posB.x) +
			(y - posB.y) * (y - posB.y) +
			(z - posB.z) * (z - posB.z));

		return distance < radius;
	}

    return false;
}
