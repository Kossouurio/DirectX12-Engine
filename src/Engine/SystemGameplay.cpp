#include "pch.h"
#include "SystemGameplay.h"

void SystemGameplay::Update(float dt, const std::vector<Entity*>& entities) {
}

void SystemGameplay::Shoot(float dt, const std::vector<Entity*>& entities) {
    float currentTime = GameEngine::GetInstance().GetTimer().TotalTime();
    if (currentTime - lastShotTime < fireRate) return;
    lastShotTime = currentTime;

    // Recuperer l'entite qui tire (actuellement, toujours le joueur)
    Entity* attacker = GameEngine::GetInstance().GetPlayerEntity();
    if (!attacker) return;

    // Recuperer la camera pour le tir (si attachee au joueur)
    ComponentTransform* cameraTransform = attacker->GetComponent<ComponentTransform>();
    if (!cameraTransform) return;

    XMFLOAT3 origin = cameraTransform->GetPosition();
    XMFLOAT3 direction = cameraTransform->GetForward();

    SystemCollider* colliderSystem = GameEngine::GetInstance().GetSystemManager().GetSystem<SystemCollider>();
    if (!colliderSystem) return;

    // Tirer un rayon pour voir si quelque chose est touche
    Entity* hitEntity = colliderSystem->Raycast(origin, direction, 100.0f, entities);
    if (hitEntity) {
        ComponentTransform* hitTransform = hitEntity->GetComponent<ComponentTransform>();
        ComponentCollider* hitCollider = hitEntity->GetComponent<ComponentCollider>();

        if (!hitTransform || !hitCollider) return;

        XMFLOAT3 entityPos = hitTransform->GetPosition();
        XMFLOAT3 colliderSize = hitCollider->GetColliderSize();

        float distance = sqrt(
            pow(entityPos.x - origin.x, 2) +
            pow(entityPos.y - origin.y, 2) +
            pow(entityPos.z - origin.z, 2)
        );

        HEAD("SHOOT") << "Ennemi touche ! ID: " << hitEntity->GetID()
            << " | Position: (" << entityPos.x << ", " << entityPos.y << ", " << entityPos.z << ")"
            << " | Collider Size: (" << colliderSize.x << ", " << colliderSize.y << ", " << colliderSize.z << ")"
            << " | Distance du tir: " << distance
            << ENDL;

        SpawnImpactEffect(hitTransform->GetPosition());

        // Appliquer les degâts si l'entite touchee a des PV
        ComponentHealth* health = hitEntity->GetComponent<ComponentHealth>();
        if (health) {
            float damage = attacker->GetComponent<ComponentHealth>()->GetDamage();
            health->TakeDamage(damage);

            // Verifier si l'entite touchee est morte
            if (health->GetHealth() <= 0) {
                ComponentScore* playerScore = attacker->GetComponent<ComponentScore>();
                if (playerScore) {
                    playerScore->AddPoints(10);
                    HEAD("SCORE") << "Score du joueur: " << playerScore->GetScore() << ENDL;
                }
            }
        }
    }
    else {
        HEAD("SHOOT") << "Aucune cible touchee !" << ENDL;
    }
}


#include <cstdlib>  // Pour rand()
#include <ctime>    // Pour initialiser l'aléatoire

void SystemGameplay::SpawnImpactEffect(XMFLOAT3 position) {
    srand(static_cast<unsigned>(time(0))); // Initialisation de l'aléatoire

    int numParticles = 3; // Nombre de particules créées

    for (int i = 0; i < numParticles; i++) {
        Entity* impactEffect = GameEngine::GetInstance().GetEntityManager().CreateEntity();
        impactEffect->AddComponent<ComponentTransform>();
        impactEffect->AddComponent<ComponentRender>();
        impactEffect->AddComponent<ParticleComponent>();

        // Récupération des composants
        ParticleComponent* particle = impactEffect->GetComponent<ParticleComponent>();
        ComponentTransform* transform = impactEffect->GetComponent<ComponentTransform>();
        ComponentPhysics* physics = impactEffect->AddComponent<ComponentPhysics>(); // Pour la vélocité

        // Durée de vie aléatoire entre 1.5 et 2.5 secondes
        particle->SetLifeTime(1.5f, 2.5f);

        // Décalage du spawn pour éviter le chevauchement avec l'entité centrale
        float offsetX = (rand() % 100 - 50) / 100.0f; // Déplacement entre -0.5 et 0.5
        float offsetY = (rand() % 100) / 200.0f;      // Légèrement au-dessus de 0
        float offsetZ = (rand() % 100 - 50) / 100.0f; // Déplacement entre -0.5 et 0.5

        XMFLOAT3 spawnPosition = {
            position.x + offsetX,
            position.y + offsetY + 0.5f, // Légèrement au-dessus pour visibilité
            position.z + offsetZ
        };
        transform->SetPosition(spawnPosition);

        // Direction et vélocité aléatoires
        float randomX = (rand() % 100 - 50) / 50.0f; // Valeur entre -1 et 1
        float randomY = (rand() % 100) / 50.0f;      // Valeur entre 0 et 2 (vers le haut)
        float randomZ = (rand() % 100 - 50) / 50.0f; // Valeur entre -1 et 1

        XMFLOAT3 velocity = { randomX * 2.0f, randomY * 3.0f, randomZ * 2.0f }; // Vitesses ajustées
        physics->SetVelocity(velocity);

        // Ajouter un scale plus petit pour que ça ressemble à des débris
        transform->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));

        HEAD("EFFECT") << "Particule " << i << " spawn à (" << spawnPosition.x << ", " << spawnPosition.y << ", " << spawnPosition.z
            << ") avec velocité (" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")" << ENDL;
    }
}
