
#pragma once

class SystemCamera : public System {
public:
    void Update(float dt, const std::vector<Entity*>& entities) override;
};
