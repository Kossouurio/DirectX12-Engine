#pragma once
//
// #include "Script.h"
//#include "UtilsImport.h"
//#include "ComponentTransform.h"


class ScriptPlayerController : public Script {
private:
    float speed = 5.0f;
    float jumpForce = 10.0f;

public:
    ScriptPlayerController(Entity* entity);

    void Update(float dt);

};

