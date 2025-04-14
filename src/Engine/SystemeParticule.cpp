#include "pch.h"
#include "SystemeParticule.h"
#include "ComponentParticule.h"
#include "ComponentTransform.h"
#include "ComponentPhysics.h"
#include "ComponentRender.h"

SystemParticle::SystemParticle() : m_initialized(false), m_buffers(nullptr) {
    // L'initialisation sera faite par GameEngine via Initialize()
}

SystemParticle::~SystemParticle() {
    // Nettoyage des ressources si necessaire
}

void SystemParticle::Initialize(ID3D12Device* device, D3D12Buffers* buffers) {
    if (!device) {
        HEAD("ERROR") << "Device is null!" << ENDL;
        return;
    }
    else if (!buffers) {
        HEAD("ERROR") << "Buffers is null!" << ENDL;
        return;
    }

    m_buffers = buffers;
    m_initialized = true;

    HEAD("sys_PARTICLE") << "Systeme de particules initialise avec succes" << ENDL;
}

void SystemParticle::Update(float dt, const std::vector<Entity*>& entities) {
    if (!m_initialized) {
        HEAD("ERROR") << "Le systeme de particules n'est pas initialise!" << ENDL;
        return;
    }


    int activeParticleCount = 0;
    for (auto* entity : entities) {
        if (!entity) continue;

        ParticleComponent* particleComp = entity->GetComponent<ParticleComponent>();
        if (particleComp && particleComp->IsActive()) {
            activeParticleCount++;
        }
    }

    // Toujours afficher le nombre de particules actives
    HEAD("sys_PARTICLE") << "Nombre de particules actives : " << activeParticleCount << ENDL;
    // Vider la liste des particules actives pour le debogage
    m_activeParticles.clear();

    // Parcourir toutes les entites avec des composants de particules
    for (auto* entity : entities) {
        if (!entity) continue;

        ParticleComponent* particleComp = entity->GetComponent<ParticleComponent>();
        if (!particleComp || !particleComp->IsActive()) continue;

        // Mettre a jour la duree de vie et la physique de la particule
        particleComp->UpdateParticle(dt);

        // Recuperer des informations pour le debogage
        if (debugMode && logSystemParticle) {
            ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
            ComponentPhysics* physics = entity->GetComponent<ComponentPhysics>();
            ComponentRender* render = entity->GetComponent<ComponentRender>();

            if (transform && render) {
                ParticleVertex pv;
                pv.position = transform->GetPosition();
                pv.velocity = physics ? physics->GetVelocity() : XMFLOAT3(0.0f, 0.0f, 0.0f);
                pv.color = render->GetColor();
                pv.size = transform->GetScale();
                pv.life = particleComp->GetLifeTime();

                m_activeParticles.push_back(pv);

                particleComp->Logs();
            }
        }

        // Si la particule n'est plus active (duree de vie epuisee), on la supprime
        if (!particleComp->IsActive()) {
            // Cette ligne depend de la facon dont tu geres la suppression des entites
            // entity->MarkForDeletion();
        }
    }

    if (debugMode && logSystemParticle) {
        HEAD("sys_PARTICLE") << "Nombre de particules actives : " << int(m_activeParticles.size()) << ENDL;
    }
}