#include "pch.h"

void SystemAudio::Update(float dt, const std::vector<Entity*>& entities) {
    for (auto* entity : entities) {
        ComponentAudio* audio = entity->GetComponent<ComponentAudio>();
        if (!audio) continue;

        // Exemple de detection d'evenements
        ComponentInput* input = entity->GetComponent<ComponentInput>();
        if (input && Input::IsKeyPressed(input->keyBindings[InputAction::JUMP])) {
            audio->PlaySound(SoundType::FOOTSTEP);
        }

        ComponentHealth* health = entity->GetComponent<ComponentHealth>();
        if (health && health->GetHealth() <= 20) { 
            audio->PlaySound(SoundType::DAMAGE);
        }
    }
}
