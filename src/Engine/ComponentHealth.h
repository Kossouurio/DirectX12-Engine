#pragma once

class ComponentHealth : public Component {
private:
    int health;
    int maxHealth;
    int damage;

public:
    ComponentHealth(Entity* entity, int hp, int dmg);

    void Logs() override;

	void SetHealth(int hp);
	int GetHealth() const;

	void SetMaxHealth(int hp);
	int GetMaxHealth() const;

	void SetDamage(int dmg);
    int GetDamage() const;
    void TakeDamage(int amount);

};
