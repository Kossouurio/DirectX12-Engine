#ifndef COMPONENTAI_H
#define COMPONENTAI_H

#include "Component.h"

class ComponentAI : public Component {
public:
	ComponentAI(Entity* entity, float moveSpeed, float attackRange, float attackCooldown);


    float GetSpeed() const;
    float GetAttackRange() const;
    bool IsChasing() const;
    float GetCooldownTime();
    float GetCooldownTimer();

    void SetChasing(bool chasing);
    void SetCooldownTimer(float time);

    void UpdateCooldown(float dt);

	void Logs() override;

private:
    float speed;
    float range;
    bool isChasing;

    float cooldownTime;  
    float cooldownTimer;
};

#endif // COMPONENTAI_H
