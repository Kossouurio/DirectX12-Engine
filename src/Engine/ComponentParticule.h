#pragma once

#include "Component.h"

enum class ParticleGeometryType {
    CUBE,
    PLANE
};

class ParticleComponent : public Component {
private:
    float m_life;
    float m_maxLife;
    bool m_active;
    ParticleGeometryType m_geometryType;

public:
    ParticleComponent(Entity* entity);
    ~ParticleComponent() = default;
    void Logs() override;

    void UpdateParticle(float dt);

    void SetLifeTime(float life, float maxLife);
    float GetLifeTime() const { return m_life; }
    float GetMaxLifeTime() const { return m_maxLife; }
    float GetLifeRatio() const { return m_life / m_maxLife; }

    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }

    ParticleGeometryType GetGeometryType() const { return m_geometryType; }
    void SetGeometryType(ParticleGeometryType type) { m_geometryType = type; }
};