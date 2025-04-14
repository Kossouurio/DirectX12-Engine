#include "pch.h"
#include "Component.h"

Component::Component(Entity* _pEntity) : m_pEntity(_pEntity) 
{
}

Component::~Component() 
{
}

int Component::GetID() const
{
	return m_pEntity->GetID();
}

Entity* Component::GetEntity()
{
	return m_pEntity;
}