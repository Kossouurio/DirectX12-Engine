#include "pch.h"
#include "ComponentPhysics.h"

ComponentPhysics::ComponentPhysics(Entity* entity) : Component(entity), vVelocity({ 0.0f, 0.0f, 0.0f }), vAcceleration({ 0.0f, 0.0f, 0.0f }) {}




void ComponentPhysics::ApplyForce(const XMFLOAT3& force) {
    vAcceleration.x += force.x;
    vAcceleration.y += force.y;
    vAcceleration.z += force.z;
}

void ComponentPhysics::ApplyGravity(float gravity) {
    if (affectedByGravity) {
        vAcceleration.y += gravity;
    }
}

void ComponentPhysics::Jump(float force) {
    if (isOnGround) {
        vVelocity.y = force;
        isOnGround = false;
    }
}


void ComponentPhysics::SetVelocity(const XMFLOAT3& vel)
{
    vVelocity = vel;
}

XMFLOAT3 ComponentPhysics::GetVelocity() const
{
    return vVelocity;
}

void ComponentPhysics::SetAcceleration(const XMFLOAT3& acc)
{
    vAcceleration = acc;
}

XMFLOAT3 ComponentPhysics::GetAcceleration() const
{
    return vAcceleration;
}

void ComponentPhysics::SetIsGrounded(bool grounded)
{
    isOnGround = grounded;
}

bool ComponentPhysics::IsOnGround() const
{
    return isOnGround;
}

void ComponentPhysics::SetaffectedByGravity(bool affected)
{
    affectedByGravity = affected;
}

bool ComponentPhysics::IsAffectedByGravity() const
{
    return affectedByGravity;
}

void ComponentPhysics::UpdatePhysics(float dt) {
    vVelocity.x += vAcceleration.x * dt;
    vVelocity.y += vAcceleration.y * dt;
    vVelocity.z += vAcceleration.z * dt;

    vAcceleration = { 0.0f, 0.0f, 0.0f };

    // Verification : si la vitesse est minuscule, on la reinitialise
    if (abs(vVelocity.x) < 0.001f) vVelocity.x = 0.0f;
    if (abs(vVelocity.z) < 0.001f) vVelocity.z = 0.0f;

    ComponentTransform* transform = GetEntity()->GetComponent<ComponentTransform>();
    if (transform) {
        XMFLOAT3 pos = transform->GetPosition();
        pos.x += vVelocity.x * dt;
        pos.y += vVelocity.y * dt;
        pos.z += vVelocity.z * dt;
        transform->SetPosition(pos);
    }
}



void ComponentPhysics::ApplyPhysics(float dt) {
    if (!affectedByGravity) return;

    if (!isOnGround) {
        vAcceleration.y = -9.81f;  // Applique la gravite
    }
    else {
        vAcceleration.y = 0.0f;
        HEAD("PHYSICS") << "Entity " << GetEntity()->GetID() << " BLOQUeE AU SOL !" << ENDL;
    }

    vVelocity.x += vAcceleration.x * dt;
    vVelocity.y += vAcceleration.y * dt;
    vVelocity.z += vAcceleration.z * dt;

    vVelocity.x *= 0.99f;  // Simule une legere friction
    vVelocity.y *= 0.99f;
    vVelocity.z *= 0.99f;

    float maxFallSpeed = -10.0f;
    if (vVelocity.y < maxFallSpeed) {
        vVelocity.y = maxFallSpeed;
    }

    vAcceleration = { 0.0f, 0.0f, 0.0f };  // Reinitialise l'acceleration apres chaque frame
}

void ComponentPhysics::Logs() {
	HEAD("PHYSICS") << "Entity ID: " << GetEntity()->GetID() << ENDL;
	HEAD("PHYSICS") << "Velocity: (" << vVelocity.x << ", " << vVelocity.y << ", " << vVelocity.z << ")" << ENDL;
	HEAD("PHYSICS") << "Acceleration: (" << vAcceleration.x << ", " << vAcceleration.y << ", " << vAcceleration.z << ")" << ENDL;
	HEAD("PHYSICS") << "On Ground: " << isOnGround << ENDL;
	HEAD("PHYSICS") << "Affected by Gravity: " << affectedByGravity << ENDL;
}