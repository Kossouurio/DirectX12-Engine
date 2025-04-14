#include "pch.h"
#include "SystemManager.h"

SystemManager::~SystemManager()
{
    for (auto* system : systems) {
        delete system;
    }
    systems.clear();
}

void SystemManager::Update(float dt) {
    if (debugMode && logSystemManager) { HEAD("CLEAN") << LINE << ENDL2; }
    if (debugMode && logSystemManager)
        HEAD("sys_MANAGER") << "Mise a jour des systemes ECS" << ENDL;

    const auto& entities = GameEngine::GetInstance().GetEntityManager().GetAllEntities();

    if (debugMode && logSystemManager)
        HEAD("sys_MANAGER") << "Nombre d'entites recuperees dans Update() : " << int(entities.size()) << ENDL;

    for (auto* system : systems) {
        if (dynamic_cast<SystemRender*>(system) == nullptr) {
            system->Update(dt, entities);
        }
    }


    if (debugMode && logSystemManager)
        HEAD("sys_MANAGER") << "Mise a jour terminee" << ENDL;
}


