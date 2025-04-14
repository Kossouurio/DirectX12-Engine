#include "pch.h"
#include "ScriptPlayerController.h"

ScriptPlayerController::ScriptPlayerController(Entity* entity)
	: Script(entity){
}

void ScriptPlayerController::Update(float dt) {
	ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
	if (!transform) return;

	float moveSpeed = speed * dt;

	//   if (Input::IsKeyPressed(KEY_LEFT)) {		
	//       HEAD("KEY") << "Left Pressed" << ENDL;
	//       transform->vVelocity.x = -moveSpeed;
	//   }
	//   else if (Input::IsKeyPressed(KEY_RIGHT)) {
	   //	HEAD("KEY") << "Right Pressed" << ENDL;
	//       transform->vVelocity.x = moveSpeed;
	   //}
	//   else {
	//       transform->vVelocity.x = 0.0f;
	//   }

	//   if (Input::IsKeyPressed(KEY_SPACE) && transform->vPosition.y <= 0.0f) {
	   //	HEAD("KEY") << "Space Pressed" << ENDL;
	//       transform->vVelocity.y = 10.0f;
	//   }

	//   if (Input::IsKeyPressed(KEY_UP)) {
	//       HEAD("KEY") << "Up Pressed" << ENDL;
	//       transform->vVelocity.y = -moveSpeed;
	//   }
	//   else if (Input::IsKeyPressed(KEY_DOWN)) {
	//       HEAD("KEY") << "Down Pressed" << ENDL;
	//       transform->vVelocity.y = moveSpeed;
	   //}
	   //else {
	   //	transform->vVelocity.y = 0.0f;
	   //}
}