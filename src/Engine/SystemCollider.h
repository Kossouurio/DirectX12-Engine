#pragma once

class Entity;
class System;
class ComponentTransform;


class SystemCollider : public System {
public:
	void Update(float dt, const std::vector<Entity*>& entities) override;

	void ProcessCollisions(const std::vector<Entity*>& entities, Entity* entityA, size_t index);
	void HandleCollision(Entity* entityA, Entity* entityB,
		ComponentTransform* transformA,
		ComponentTransform* transformB);
	void ApplyCollisionResponse(ComponentTransform* transformA,
		ComponentTransform* transformB,
		const XMFLOAT3& direction);

	Entity* Raycast(XMFLOAT3 origin, XMFLOAT3 direction, float maxDistance, const std::vector<Entity*>& entities);

};
