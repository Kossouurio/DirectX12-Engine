
#pragma once

class System;

class SystemAudio : public System {
public:
    void Update(float dt, const std::vector<Entity*>& entities) override;
};
