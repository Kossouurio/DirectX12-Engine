#ifndef SYSTEMAI_H
#define SYSTEMAI_H

#include "System.h"

class SystemAI : public System {
public:
    void Update(float dt, const std::vector<Entity*>& entities) override;
};

#endif // SYSTEMAI_H
