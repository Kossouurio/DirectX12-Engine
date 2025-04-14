#include "pch.h"
#include "SystemAI.h"

void SystemAI::Update(float dt, const std::vector<Entity*>& entities) {
    Entity* player = GameEngine::GetInstance().GetPlayerEntity();
    if (!player) return;

    ComponentTransform* playerTransform = player->GetComponent<ComponentTransform>();
    if (!playerTransform) return;

    XMFLOAT3 playerPos = playerTransform->GetPosition();

    for (Entity* entity : entities) {
        if (!entity->GetComponent<ComponentAI>()) continue;

        ComponentAI* ai = entity->GetComponent<ComponentAI>();
        ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
        ComponentPhysics* physics = entity->GetComponent<ComponentPhysics>();

        if (!ai || !transform || !physics) continue;

        XMFLOAT3 enemyPos = transform->GetPosition();

        // Mettre a jour le cooldown d'attaque
        ai->UpdateCooldown(dt);

        // Si le cooldown est termine, reprendre la poursuite
        if (ai->GetCooldownTimer() <= 0 && !ai->IsChasing()) {
            ai->SetChasing(true);
            HEAD("AI") << "Ennemi ID: " << entity->GetID() << " reprend la poursuite !" << ENDL;
        }


        // Calcul de la distance au joueur
        float distance = sqrt(
            pow(playerPos.x - enemyPos.x, 2) +
            pow(playerPos.y - enemyPos.y, 2) +
            pow(playerPos.z - enemyPos.z, 2)
        );

        // Si l'ennemi est en cooldown, il reste immobile
        if (ai->GetCooldownTimer() > 0) {
            physics->SetVelocity({ 0.0f, physics->GetVelocity().y, 0.0f });
            continue;
        }

        // Si l'ennemi est trop proche, il attaque et declenche le cooldown
        if (distance <= ai->GetAttackRange()) {
            ai->SetChasing(false);
            ai->SetCooldownTimer(ai->GetCooldownTime()); // Declenche l'arrêt temporaire
            ComponentHealth* playerHealth = player->GetComponent<ComponentHealth>();
            if (playerHealth) {
                playerHealth->TakeDamage(10.0f * dt); // Inflige des degâts au joueur
            }

            // Arrête le mouvement pendant le cooldown
            physics->SetVelocity({ 0.0f, physics->GetVelocity().y, 0.0f });
            continue;
        }

        // Si l'ennemi peut bouger, on met a jour la direction a chaque frame
        if (ai->IsChasing()) {
            float speed = ai->GetSpeed();

            XMFLOAT3 direction = {
                (playerPos.x - enemyPos.x) / distance,
                0, // Ne pas influencer l'axe Y
                (playerPos.z - enemyPos.z) / distance
            };

            // Appliquer une force mise a jour en permanence
            XMFLOAT3 newVelocity = {
                direction.x * speed,
                physics->GetVelocity().y, // Conserver la gravite
                direction.z * speed
            };

            physics->SetVelocity(newVelocity);
        }
    }
}
