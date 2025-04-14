#pragma once
#include "System.h"
#include "SystemGameplay.h"

class SystemInput : public System {
public:
    void Update(float dt, const std::vector<Entity*>& entities);

    void SetGameplaySystem(SystemGameplay* gameplaySystem); //TO delete
private:
    SystemGameplay* gameplay = nullptr; //TO delete

    void Moves(ComponentInput* input, ComponentTransform* transform, float dt);
    void MouseRotation(ComponentTransform* transform);
    void KeyRotation(ComponentInput* input, ComponentTransform* transform, float dt);
    void ResetMoves(ComponentInput* input, ComponentPhysics* physics);
    void Action(ComponentInput* input, ComponentAudio* audio, std::vector<Entity*> entities, float dt);
};




