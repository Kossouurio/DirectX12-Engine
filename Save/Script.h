#pragma once
#include "Entity.h"

class Script {
protected:
    Entity* entity;

public:
    Script(Entity* entity) : entity(entity) {}
    virtual ~Script() = default;

    virtual void Start() {}  
    virtual void Update(float dt) = 0; 
};
