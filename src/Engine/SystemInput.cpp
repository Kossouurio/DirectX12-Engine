#include "pch.h"
#include "SystemInput.h"

void SystemInput::Update(float dt, const std::vector<Entity*>& entities) {
    for (auto* entity : entities) {
        ComponentInput* input = entity->GetComponent<ComponentInput>();
        ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
        ComponentPhysics* physics = entity->GetComponent<ComponentPhysics>();
        ComponentAudio* audio = entity->GetComponent<ComponentAudio>();

        if (!input || !transform || !physics) continue;

        if (GameEngine::GetInstance().IsCameraLocked()) {
            // Ne pas traiter les inputs de mouvement de caméra
            return;
        }

        Moves(input, transform, dt);

        if (MouseCamera) {
            MouseRotation(transform);
        }
        else {
            KeyRotation(input, transform, dt);
        }

		ResetMoves(input, physics);
        Action(input, audio, entities, dt);
    }
}

void SystemInput::SetGameplaySystem(SystemGameplay* gameplaySystem) {
    gameplay = gameplaySystem;
}

void SystemInput::Moves(ComponentInput* input, ComponentTransform* transform, float dt) {
    float moveSpeed = 5.0f * dt;
    XMFLOAT3 forward = transform->GetForward();
    XMFLOAT3 right = transform->GetRight();

    XMFLOAT3 positionFloat = transform->GetPosition(); 
    XMVECTOR position = XMLoadFloat3(&positionFloat); 

    if (Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_FRONTWARD])) {
        position = XMVectorAdd(position, XMVectorScale(XMLoadFloat3(&forward), moveSpeed));
    }
    if (Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_BACKWARD])) {
        position = XMVectorSubtract(position, XMVectorScale(XMLoadFloat3(&forward), moveSpeed));
    }
    if (Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_LEFT])) {
        position = XMVectorSubtract(position, XMVectorScale(XMLoadFloat3(&right), moveSpeed));
    }
    if (Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_RIGHT])) {
        position = XMVectorAdd(position, XMVectorScale(XMLoadFloat3(&right), moveSpeed));
    }

    if (debugMove) {
        if (Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_UP])) {
            position = XMVectorAdd(position, XMVectorSet(0.0f, moveSpeed, 0.0f, 0.0f));
        }
        if (Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_DOWN])) {
            position = XMVectorAdd(position, XMVectorSet(0.0f, -moveSpeed, 0.0f, 0.0f));
        }
    }

    XMFLOAT3 newPosition;
    XMStoreFloat3(&newPosition, position);
    transform->SetPosition(newPosition);
}

void SystemInput::MouseRotation(ComponentTransform* transform) {
    // Sensibilite
    float sensitivity = 0.02f;

    // Recuperation des mouvements de la souris
    float deltaX = Input::GetMouseDeltaX();
    float deltaY = Input::GetMouseDeltaY();

    // Calcul du yaw (rotation horizontale)
    float yawRotation = deltaX * sensitivity;

    // Calcul du pitch (rotation verticale)
    static float accumulatedPitch = 0.0f;
    float pitchRotation = deltaY * sensitivity;

    // Limiter l'inclinaison de la camera
    const float maxPitch = XM_PIDIV2 - 0.01f;
    const float minPitch = -XM_PIDIV2 + 0.01f;

    float newPitch = accumulatedPitch + pitchRotation;
    if (newPitch > maxPitch) {
        pitchRotation = maxPitch - accumulatedPitch;
        accumulatedPitch = maxPitch;
    }
    else if (newPitch < minPitch) {
        pitchRotation = minPitch - accumulatedPitch;
        accumulatedPitch = minPitch;
    }
    else {
        accumulatedPitch = newPitch;
    }

    // Appliquer la rotation a la camera
    transform->RotateAroundAxis(XMFLOAT3(0.0f, 1.0f, 0.0f), yawRotation);
    transform->RotateAroundAxis(transform->GetRight(), pitchRotation);
}

void SystemInput::KeyRotation(ComponentInput* input, ComponentTransform* transform, float dt) {
    float rotationSpeed = 1.0f * dt;
    float yawRotation = 0.0f;
    float pitchRotation = 0.0f;

    if (Input::IsKeyPressed(input->keyBindings[InputAction::ROTATE_LEFT])) {
        yawRotation -= rotationSpeed;
    }
    if (Input::IsKeyPressed(input->keyBindings[InputAction::ROTATE_RIGHT])) {
        yawRotation += rotationSpeed;
    }
    if (Input::IsKeyPressed(input->keyBindings[InputAction::ROTATE_UP])) {
        pitchRotation -= rotationSpeed;
    }
    if (Input::IsKeyPressed(input->keyBindings[InputAction::ROTATE_DOWN])) {
        pitchRotation += rotationSpeed;
    }

    // Appliquer la rotation YAW (Rotation horizontale)
    if (yawRotation != 0.0f) {
        transform->RotateAroundAxis(XMFLOAT3(0.0f, 1.0f, 0.0f), yawRotation);
    }

    // Appliquer la rotation PITCH (Rotation verticale) avec clamp
    if (pitchRotation != 0.0f) {
        static float accumulatedPitch = 0.0f;

        // Limite la rotation verticale
        const float maxPitch = XM_PIDIV2 - 0.01f;
        const float minPitch = -XM_PIDIV2 + 0.01f;

        float newPitch = accumulatedPitch + pitchRotation;

        if (newPitch > maxPitch) {
            pitchRotation = maxPitch - accumulatedPitch;
            accumulatedPitch = maxPitch;
        }
        else if (newPitch < minPitch) {
            pitchRotation = minPitch - accumulatedPitch;
            accumulatedPitch = minPitch;
        }
        else {
            accumulatedPitch = newPitch;
        }

        if (pitchRotation != 0.0f) {
            transform->RotateAroundAxis(transform->GetRight(), pitchRotation);
        }
    }
}

void SystemInput::ResetMoves(ComponentInput* input, ComponentPhysics* physics) {
    if (!Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_LEFT]) &&
        !Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_RIGHT])) {
        physics->SetVelocity(XMFLOAT3(0.0f, physics->GetVelocity().y, physics->GetVelocity().z));
    }

    if (!Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_FRONTWARD]) &&
        !Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_BACKWARD])) {
        physics->SetVelocity(XMFLOAT3(physics->GetVelocity().x, physics->GetVelocity().y, 0.0f));
    }

    if (debugMove) {
        if (!Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_UP]) &&
            !Input::IsKeyPressed(input->keyBindings[InputAction::MOVE_DOWN])) {
            physics->SetVelocity(XMFLOAT3(physics->GetVelocity().x, 0.0f, physics->GetVelocity().z));
        }
    }
    else {
        if (Input::IsKeyPressed(input->keyBindings[InputAction::JUMP])) {
            physics->Jump(10.0f);
        }
    }
}

void SystemInput::Action(ComponentInput* input, ComponentAudio* audio, std::vector<Entity*> entities, float dt) {
    if (!input) return;

    if (Input::IsKeyPressed(input->keyBindings[InputAction::JUMP])) {
        /*audio->PlaySound(SoundType::FOOTSTEP);*/
    }

    if (Input::IsKeyPressed(input->keyBindings[InputAction::SHOOT])) {

        /*audio->PlaySound(SoundType::SHOOT);*/

        // Verifie que `SystemGameplay` est bien defini
        if (gameplay) {
            gameplay->Shoot(dt, entities);
        }
    }
}
