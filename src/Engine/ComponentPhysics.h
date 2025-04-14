#pragma once
//#include "Component.h"

class ComponentPhysics : public Component {
private:
    XMFLOAT3 vVelocity;
    XMFLOAT3 vAcceleration;

    bool isOnGround = false;
    bool affectedByGravity = true;

public:
    ComponentPhysics(Entity* entity);

    void ApplyForce(const XMFLOAT3& force);
    void ApplyGravity(float gravity);
    void UpdatePhysics(float dt);
	void ApplyPhysics(float dt);

	void Jump(float force);

    XMFLOAT3 GetVelocity() const;
    void SetVelocity(const XMFLOAT3& v);

    XMFLOAT3 GetAcceleration() const;
    void SetAcceleration(const XMFLOAT3& a);

    bool IsOnGround() const;
    void SetIsGrounded(bool state);

    bool IsAffectedByGravity() const;
    void SetaffectedByGravity(bool state);

	void Logs();
};
