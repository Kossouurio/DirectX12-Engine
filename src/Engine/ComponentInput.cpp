#include "pch.h"
#include "ComponentInput.h"

ComponentInput::ComponentInput(Entity* entity) : Component(entity) {
    keyBindings[InputAction::MOVE_FRONTWARD] = KEY_Z;  // Avancer (Z)
    keyBindings[InputAction::MOVE_BACKWARD] = KEY_S;   // Reculer (S)
    keyBindings[InputAction::MOVE_LEFT] = KEY_Q;  // Gauche (Q)
    keyBindings[InputAction::MOVE_RIGHT] = KEY_D; // Droite (D)

    keyBindings[InputAction::MOVE_UP] = KEY_SPACE; // Monter (Espace)
    keyBindings[InputAction::MOVE_DOWN] = KEY_CTRL; // Descendre (Ctrl)

    keyBindings[InputAction::ROTATE_LEFT] = KEY_LEFT; // Rotation gauche (Fleche gauche)
    keyBindings[InputAction::ROTATE_RIGHT] = KEY_RIGHT; // Rotation droite (Fleche droite)
    keyBindings[InputAction::ROTATE_UP] = KEY_UP; // Rotation haut (Fleche haut)
    keyBindings[InputAction::ROTATE_DOWN] = KEY_DOWN; // Rotation bas (Fleche bas)

    keyBindings[InputAction::JUMP] = KEY_SPACE; // Saut (Espace)
	keyBindings[InputAction::SHOOT] = VK_LBUTTON; // Tir (Clic gauche)
	keyBindings[InputAction::INTERACT] = KEY_E; // Interaction (E)µ
	keyBindings[InputAction::SET] = KEY_P; // Debug (P)
}
