#include "pch.h"
#include "ComponentParticule.h"
#include "ComponentTransform.h"
#include "ComponentPhysics.h"
#include "ComponentRender.h"

ParticleComponent::ParticleComponent(Entity* entity)
    : Component(entity), m_active(true), m_life(500.0f), m_maxLife(500.0f), m_geometryType(ParticleGeometryType::CUBE) {}

void ParticleComponent::UpdateParticle(float dt) {
    if (!m_active) return;

    // Gestion de la duree de vie
    m_life -= dt;
    if (m_life <= 0) {
        m_active = false;
        return;
    }

    // Recuperation des composants necessaires
    ComponentTransform* transform = m_pEntity->GetComponent<ComponentTransform>();
    ComponentPhysics* physics = m_pEntity->GetComponent<ComponentPhysics>();
    ComponentRender* render = m_pEntity->GetComponent<ComponentRender>();

    if (!transform || !render) return;

    // Application d'effets visuels selon la duree de vie
    float lifeRatio = m_life / m_maxLife;

    // Effet de transparence progressive
    XMFLOAT4 color = render->GetColor();
    color.w = lifeRatio; // Alpha diminue progressivement
    render->SetColor(color);
}

void ParticleComponent::SetLifeTime(float life, float maxLife) {
    m_life = life;
    m_maxLife = maxLife;
}

void ParticleComponent::Logs() {
    if (!m_active) return;

    ComponentTransform* transform = m_pEntity->GetComponent<ComponentTransform>();
    XMFLOAT3 position = transform ? transform->GetPosition() : XMFLOAT3(0, 0, 0);

    HEAD("sys_PARTICLE") << "Particle Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << ENDL;
    HEAD("sys_PARTICLE") << "Life: " << m_life << "/" << m_maxLife << " (" << (m_life / m_maxLife * 100) << "%)" << ENDL;
    HEAD("sys_PARTICLE") << "Active: " << (m_active ? "true" : "false") << ENDL;
}