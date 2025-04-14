#pragma once
//#include "Component.h"

enum class InputAction {
    MOVE_LEFT,
    MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,

	MOVE_FRONTWARD,
	MOVE_BACKWARD,

	ROTATE_LEFT,
	ROTATE_RIGHT,
	ROTATE_UP,
	ROTATE_DOWN,

	SHOOT,
    JUMP,
	INTERACT,
	SET
};

class ComponentInput : public Component {
public:
	std::map<InputAction, int> keyBindings;

    ComponentInput(Entity* entity);

};