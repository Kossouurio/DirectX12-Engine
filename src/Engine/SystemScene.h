#ifndef SYSTEMSCENE_H
#define SYSTEMSCENE_H


#include "System.h"
#include "EntityManager.h"
#include "D3D12Window.h"

class SystemScene : public System {
public:
	SystemScene() {}

    void InitScene();
    void CreateCrosshair();

    int GetCrosshairID() const { return crosshairID; }
    void Update(float dt, const std::vector<Entity*>& entities) override;

private:
    int crosshairID = -1;

    void CreatePlayer();
    void CreateSkybox();


    void CreateGroundAndWalls();
    void CreateObstacles();
    void SpawnEnemies();
};

#endif // SYSTEMSCENE_H