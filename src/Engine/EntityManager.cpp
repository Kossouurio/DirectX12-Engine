#include "pch.h"
#include "EntityManager.h"

EntityManager::~EntityManager() {
    HEAD("EntityManager") << "Destructeur appele ! Nombre d'entites avant suppression : " << int(entities.size()) << ENDL;
}


Entity* EntityManager::CreateEntity() {
    Entity* newEntity = new Entity(nextEntityID);
    entities.insert({ nextEntityID, newEntity });

    //HEAD("EntityManager") << "Entite creee - ID: " << newEntity->GetID()
        //<< " | Nombre total apres ajout: " << int(entities.size()) << ENDL;

    nextEntityID++;
    return newEntity;
}



Entity* EntityManager::GetEntity(int id) {
    auto it = entities.find(id);
    return (it != entities.end()) ? it->second : nullptr;
}

std::vector<Entity*> EntityManager::GetAllEntities() {
    std::vector<Entity*> entityList;

    for (auto& pair : entities) {
        entityList.push_back(pair.second);
    }


	SetEntityList(entityList);
    return entityList;
}

void EntityManager::SetEntityList(std::vector<Entity*> entityList) {
	entityList = entityList;
}

std::vector<Entity*> EntityManager::GetEntityList() {
	return entityList;
}

void EntityManager::DestroyEntity(int id) {
    auto it = entities.find(id);
    if (it != entities.end()) {
        delete it->second;
        entities.erase(it);
    }
}

void EntityManager::Clear() {
    for (auto& pair : entities) {
        delete pair.second;
    }
    entities.clear();
}
