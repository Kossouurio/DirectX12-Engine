#include "pch.h"
#include "ComponentInput.h"

ComponentInput::ComponentInput(Entity* entity) : Component(entity) {
    keyBindings[InputAction::MOVE_LEFT] = 37;  // Gauche
    keyBindings[InputAction::MOVE_RIGHT] = 39; // Droite
    keyBindings[InputAction::JUMP] = 32;       // Espace
}

void ComponentInput::BindKey(InputAction action, int key) {
    keyBindings[action] = key;
}
