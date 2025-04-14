#include "pch.h"
#include "SystemCamera.h"

void SystemCamera::AddEntity(Entity* entity) {
    if (entity->GetComponent<ComponentCamera>()) {
        entities.push_back(entity);
		HEAD("SYSTEM") << "Entite" << " ajoutee au systeme de camera" << ENDL;
    }
}

void SystemCamera::Update(float dt) {
    for (auto* entity : entities) {
        ComponentCamera* camera = entity->GetComponent<ComponentCamera>();
        if (!camera) continue;

        float speed = 5.0f * dt;
        float lookSpeed = 0.1f * dt;

        if (Input::IsKeyPressed('W')) camera->Move({ 0.0f, 0.0f, speed });
        if (Input::IsKeyPressed('S')) camera->Move({ 0.0f, 0.0f, -speed });
        if (Input::IsKeyPressed('A')) camera->Move({ -speed, 0.0f, 0.0f });
        if (Input::IsKeyPressed('D')) camera->Move({ speed, 0.0f, 0.0f });

        HEAD("CAMERA") << "Position: (" << camera->position.x << ", " << camera->position.y << ", " << camera->position.z << ")" << ENDL;

        //float mouseX = Input::GetMouseDeltaX();
        //float mouseY = Input::GetMouseDeltaY();
        //camera->Rotate({ -mouseY * lookSpeed, mouseX * lookSpeed, 0.0f });
    }
}
