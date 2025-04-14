#include "pch.h"
#include "SystemRender.h"

void SystemRender::Update(float dt, const std::vector<Entity*>& entities) {
    if (debugMode && logSystemRender) {
        HEAD("CLEAN") << LINE << ENDL2;
    }

    // Separer les entites normales et les particules
    std::vector<Entity*> regularEntities;
    std::vector<Entity*> particleEntities;

    for (auto* entity : entities) {
        if (!entity) continue;

        ComponentRender* render = entity->GetComponent<ComponentRender>();
        if (!render || !render->GetVisibility()) continue;

        ParticleComponent* particle = entity->GetComponent<ParticleComponent>();
        if (particle && particle->IsActive()) {
            particleEntities.push_back(entity);
        }
        else {
            regularEntities.push_back(entity);
        }
    }

    // Rendre d'abord les entites normales
    for (auto* entity : regularEntities) {
        ComponentRender* render = entity->GetComponent<ComponentRender>();
        render->Render(dt);

        if (debugMode && logSystemRender) {
            render->Logs();
        }
    }

    Entity* crosshair = GameEngine::GetInstance().GetEntityManager().GetEntity(
        GameEngine::GetInstance().GetSystemManager().GetSystem<SystemScene>()->GetCrosshairID()
    );

    Entity* player = GameEngine::GetInstance().Getplayer();
    if (crosshair && player) {
        ComponentTransform* crosshairTransform = crosshair->GetComponent<ComponentTransform>();
        ComponentTransform* cameraTransform = player->GetComponent<ComponentTransform>();

        if (crosshairTransform && cameraTransform) {
            XMFLOAT3 camPos = cameraTransform->GetPosition();
            XMFLOAT3 camForward = cameraTransform->GetForward();

            // Positionner le viseur devant la camera
            XMFLOAT3 crosshairPos = {
                camPos.x + camForward.x * 2.0f, // Ajuster la distance
                camPos.y + camForward.y * 2.0f,
                camPos.z + camForward.z * 2.0f
            };

            crosshairTransform->SetPosition(crosshairPos);
        }
    }

    // Ensuite, rendre les particules
    if (!particleEntities.empty()) {
        // Configurer le pipeline pour les particules (blending, etc.)
        D3D12Window* window = GameEngine::GetInstance().GetWindow();
        ID3D12GraphicsCommandList* cmdList = window->GetCommandList();

     

        for (auto* entity : particleEntities) {
            ComponentRender* render = entity->GetComponent<ComponentRender>();
            render->Render(dt);

            if (debugMode && logSystemRender) {
                render->Logs();

                ParticleComponent* particle = entity->GetComponent<ParticleComponent>();
                ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
                HEAD("PARTICLE_RENDER") << "Rendering Particle ID: " << entity->GetID()
                    << ", Position: (" << transform->GetPosition().x
                    << ", " << transform->GetPosition().y
                    << ", " << transform->GetPosition().z << ")"
                    << ", Life: " << particle->GetLifeRatio() * 100.0f << "%" << ENDL;
            }
        }
    }
    
}