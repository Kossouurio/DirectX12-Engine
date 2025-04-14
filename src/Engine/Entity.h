#pragma once

class Component;
class System;
class Script;

class Entity {
private:
    int m_id;
    std::vector<Component*> m_components;
	std::vector<System*> m_systems;
	std::list<Script*> m_script;

public:
    Entity(int id);

    int GetID() const;

    template <typename T, typename... Args>
    T* AddComponent(Args&&... args);

    template <typename T>
    T* GetComponent();

    template <typename T>
    void RemoveComponent();

	void Destroy();

    void Update(float dt);

};

#include "Entity.inl"