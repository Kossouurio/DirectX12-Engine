#include "pch.h"
#include "Entity.h"

Entity::Entity(int id) : m_id(id) {}

int Entity::GetID() const {
	return m_id;
}

void Entity::Update(float dt) {}

void Entity::Destroy() {
    // Supprimer explicitement chaque type de composant connu
    RemoveComponent<ComponentTransform>();
    RemoveComponent<ComponentRender>();
    RemoveComponent<ComponentCollider>();
    RemoveComponent<ComponentMesh>();
    RemoveComponent<ComponentPhysics>();
    RemoveComponent<ComponentCamera>();
    RemoveComponent<ComponentInput>();

    // Supprime l'entite du gestionnaire d'entites
    GameEngine::GetInstance().GetEntityManager().DestroyEntity(m_id);
}
