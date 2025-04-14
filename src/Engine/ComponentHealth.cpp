#include "pch.h"
#include "ComponentHealth.h"

ComponentHealth::ComponentHealth(Entity* entity, int hp, int dmg) : Component(entity), health(hp), maxHealth(hp), damage(dmg) 
{
}

void ComponentHealth::Logs() {
	HEAD("sys_HEALTH") << "Entite " << GetEntity()->GetID() << " : "
		<< health << " / " << maxHealth << " HP" << ENDL;
}


void ComponentHealth::SetHealth(int hp) 
{
	health = hp;
}

int ComponentHealth::GetHealth() const 
{
	return health;
}

void ComponentHealth::SetMaxHealth(int hp) 
{
	maxHealth = hp;
}

int ComponentHealth::GetMaxHealth() const 
{
	return maxHealth;
}

void ComponentHealth::SetDamage(int dmg) 
{
	damage = dmg;
}

int ComponentHealth::GetDamage() const 
{
	return damage;
}

void ComponentHealth::TakeDamage(int amount) {
	health -= amount;
	if (health <= 0) {
		GetEntity()->GetComponent<ComponentRender>()->SetVisibility(false);
		GetEntity()->GetComponent<ComponentCollider>()->SetActive(false);

		/*GetEntity()->Destroy();*/
	}
}
