#pragma once

class EntityManager {
private:
    std::map<int, Entity*> entities;
	std::vector<Entity*> entityList;
    int nextEntityID = 0;  

public:
	~EntityManager();

    Entity* CreateEntity();

    Entity* GetEntity(int id);
    std::vector<Entity*> GetAllEntities();

	void SetEntityList(std::vector<Entity*> entityList);
	std::vector<Entity*> GetEntityList();

    void DestroyEntity(int id);
    void Clear();
};
