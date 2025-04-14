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

void Component::Logs() {
	HEAD("COMPONENT") << "No Logs for Component ID: " << GetID() << ENDL;
}

Entity* Component::GetEntity()
{
	if (m_pEntity) {
		return m_pEntity;
	}
}