#pragma once
//#include "Component.h"

using namespace DirectX;

enum class ColliderType {
    AABB,   
    SPHERE  
};

class ComponentCollider : public Component {
private:
    ColliderType colliderType;
    XMFLOAT3 colliderSize;  
    bool isTrigger; 

	bool isActive = true;

public:
    ComponentCollider(Entity* entity, ColliderType type, const XMFLOAT3& size, bool trigger = false);

    void Logs() override;

    void SetColliderType(ColliderType type);
	ColliderType GetColliderType() const;

    void SetColliderSize(const XMFLOAT3& size);
	XMFLOAT3 GetColliderSize() const;

	void SetActive(bool active);

    void UpdateColliderSize();

    void SetTrigger(bool trigger);
	bool IsTrigger() const;

    bool CheckCollision(ComponentCollider* other); 

};
