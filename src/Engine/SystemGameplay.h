#pragma once

class SystemGameplay : public System {
public:
    void Update(float dt, const std::vector<Entity*>& entities) override; 
    void Shoot(float dt, const std::vector<Entity*>& entities);  
    void SpawnImpactEffect(XMFLOAT3 position);

private:
    float lastShotTime = 0.0f;
    float fireRate = 0.2f;
};
