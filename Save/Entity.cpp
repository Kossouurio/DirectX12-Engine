#include "pch.h"
#include "Entity.h"

Entity::Entity(int id) : m_id(id) {}

Entity::~Entity() {
    Destroy();
}

int Entity::GetID() const {
	return m_id;
}

void Entity::Update(float dt) {
	for (auto& system : m_systems) {
		system->Update(dt);
	}
}

void Entity::Destroy() {
    for (auto component : m_components) { 
        delete component; 
    }
    m_components.clear();
}


