#include "pch.h"
#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(Entity* entity)
	: Component(entity)
{

}

ComponentMesh::~ComponentMesh()
{
}

int ComponentMesh::GetID() const
{
	return m_pEntity->GetID();
}
