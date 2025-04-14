#pragma once
//#include "System.h"
//#include "ComponentTransform.h"

class SystemTransform : public System {
public:
    void Update(float dt, const std::vector<Entity*>& entities) override;
};
