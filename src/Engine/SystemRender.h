#pragma once
//#include "System.h"
//#include "ComponentRender.h"

class SystemRender : public System {
private:
    XMMATRIX mViewMatrix = XMMatrixIdentity();
    XMMATRIX mProjMatrix = XMMatrixIdentity();
public:

    void Update(float dt, const std::vector<Entity*>& entities) override;
};
