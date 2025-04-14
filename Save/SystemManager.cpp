#include "pch.h"
#include "SystemManager.h"

SystemManager::~SystemManager()
{
    for (auto* system : systems) {
        delete system;
    }
    systems.clear();
}

void SystemManager::AddSystem(System* system)
{
    systems.push_back(system);
}

void SystemManager::Update(float dt)
{
    for (auto* system : systems) {
        system->Update(dt);
    }
}
