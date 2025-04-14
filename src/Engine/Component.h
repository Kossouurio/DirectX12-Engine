#pragma once


class Entity;

class Component {
protected:
    Entity* m_pEntity;

public:
    Component(Entity* _pEntity);
    virtual ~Component();

	int GetID() const;
	virtual void Logs();

    Entity* GetEntity();
};

