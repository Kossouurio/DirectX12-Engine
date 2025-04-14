#include "pch.h"
#include "ComponentAI.h"

ComponentAI::ComponentAI(Entity* entity, float moveSpeed, float attackRange, float attackCooldown)
	: Component(entity), speed(moveSpeed), range(attackRange), cooldownTime(attackCooldown), cooldownTimer(0.0f), isChasing(true) {
}
float ComponentAI::GetSpeed() const {
	return speed;
}

float ComponentAI::GetAttackRange() const {
	return range;
}

bool ComponentAI::IsChasing() const {
	return isChasing;
}

void ComponentAI::SetChasing(bool chasing) {
	isChasing = chasing;
}

float ComponentAI::GetCooldownTime() {
	return cooldownTime;
}

float ComponentAI::GetCooldownTimer() {
	return cooldownTimer;
}

void ComponentAI::SetCooldownTimer(float time) {
	cooldownTimer = time;
}

void ComponentAI::UpdateCooldown(float dt) {
	if (cooldownTimer > 0) {
		cooldownTimer -= dt;
	}
}




void ComponentAI::Logs() {
	HEAD("COMPONENT AI") << "Speed: " << speed << " | Range: " << range << " | Chasing: " << isChasing << ENDL;
}

