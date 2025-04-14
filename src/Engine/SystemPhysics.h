#pragma once
//#include "UtilsImport.h"
//#include "UtilsMain.h"

class SystemPhysics : public System {
private:
    XMFLOAT3 gravity = { 0.0f, -9.81f, 0.0f };

public:
    void Update(float dt, const std::vector<Entity*>& entities) override;
};
