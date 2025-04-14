#include "pch.h"
#include "SystemScene.h"

void SystemScene::InitScene() {
    HEAD("SCENE") << "Initialisation de la scene FPS..." << ENDL;
    CreateGroundAndWalls();
    CreateObstacles();
    SpawnEnemies();
    CreatePlayer();
    /*CreateSkybox();*/
    HEAD("SCENE") << "Scene FPS prête !" << ENDL;
}

// Deplacement du joueur ici
void SystemScene::CreatePlayer() {
    Entity* player = GameEngine::GetInstance().GetEntityManager().CreateEntity();
    player->AddComponent<ComponentTransform>();
    player->AddComponent<ComponentRender>();
    player->AddComponent<ComponentCollider>(ColliderType::AABB, XMFLOAT3{ 1.0f, 1.8f, 1.0f }); // Hauteur ajustee pour FPS
    player->AddComponent<ComponentPhysics>();
    player->AddComponent<ComponentMesh>(GameEngine::GetInstance().GetWindow()->GetDevice(), GeometryGenerator::GenerateSphere(1.0f, 10.0f));
	player->AddComponent<ComponentHealth>(100, 50);

    ComponentMesh* playerMesh = player->GetComponent<ComponentMesh>();
    playerMesh->GetMesh()->SetColor(0, 0, 1);
    ComponentTransform* playerTransform = player->GetComponent<ComponentTransform>();
    playerTransform->SetPosition(XMFLOAT3(0.0f, 1.8f, 0.0f)); 

    // Ajout de la camera au joueur 
    player->AddComponent<ComponentCamera>();
    player->AddComponent<ComponentInput>();

    ComponentCamera* cam = player->GetComponent<ComponentCamera>();
    if (!cam) {
        HEAD("ERROR") << "Le joueur ne contient pas de ComponentCamera !" << ENDL;
    }
    else {
        GameEngine::GetInstance().GetWindow()->SetCameraComponent(cam);
    }

    // Enregistre le joueur dans le moteur
    GameEngine::GetInstance().SetPlayerEntity(player);
}

// Creation du sol et des murs invisibles
void SystemScene::CreateGroundAndWalls() {
    // Sol
    Entity* ground = GameEngine::GetInstance().GetEntityManager().CreateEntity();
    ground->AddComponent<ComponentTransform>();
    ground->AddComponent<ComponentRender>();
    ground->AddComponent<ComponentCollider>(ColliderType::AABB, XMFLOAT3{ 100.0f, 1.0f, 100.0f });
    ground->AddComponent<ComponentMesh>(GameEngine::GetInstance().GetWindow()->GetDevice(), GeometryGenerator::GeneratePlane(100.0f, 100.0f));

	ComponentMesh* groundMesh = ground->GetComponent<ComponentMesh>();
	groundMesh->GetMesh()->SetColor(0.4f,0.4f, 0.4f); // Couleur grise

    ComponentTransform* groundTransform = ground->GetComponent<ComponentTransform>();
    groundTransform->SetPosition(XMFLOAT3(0.0f, -3.0f, 0.0f));
    groundTransform->RotatePitch(XMConvertToRadians(180.0f)); // Rotation du sol

}


#include <cstdlib>  // Pour rand()
#include <ctime>    // Pour seed aleatoire

#include <cstdlib>  // Pour rand()
#include <ctime>    // Pour seed aleatoire

void SystemScene::CreateObstacles() {
    srand(static_cast<unsigned>(time(0))); // Initialise la seed aleatoire

    int numObstacles = 20; // Nombre total d'obstacles
    float levelSize = 70.0f; // Taille de la zone jouable

    for (int i = 0; i < numObstacles; i++) {
        // Generer une position aleatoire dans la zone de jeu
        float posX = (rand() % static_cast<int>(levelSize)) - (levelSize / 2);
        float posZ = (rand() % static_cast<int>(levelSize)) - (levelSize / 2);
        float posY = 0.0f; // Toujours sur le sol

        // Determiner aleatoirement l'orientation du mur
        bool isHorizontal = rand() % 2; // 50% de chance d'être un mur X ou Z

        // Generer une taille aleatoire mais contrôlee pour eviter des cubes
        float scaleX, scaleZ;
        if (isHorizontal) {
            scaleX = 4.0f + (rand() % 10); // Entre 4 et 10 (mur large)
            scaleZ = 0.5f + (rand() % 1); // Tres fin (mur allonge)
        }
        else {
            scaleX = 0.5f + (rand() % 1); // Tres fin (mur allonge)
            scaleZ = 4.0f + (rand() % 10); // Entre 4 et 10 (mur large)
        }

        float scaleY = 7.0f;// Hauteur variable entre 2 et 7

        // Creer l'entite obstacle
        Entity* obstacle = GameEngine::GetInstance().GetEntityManager().CreateEntity();
        obstacle->AddComponent<ComponentTransform>();
        obstacle->AddComponent<ComponentRender>();
        obstacle->AddComponent<ComponentCollider>(ColliderType::AABB, XMFLOAT3(scaleX, scaleY, scaleZ));
        obstacle->AddComponent<ComponentMesh>(GameEngine::GetInstance().GetWindow()->GetDevice(), GeometryGenerator::GenerateCube(2.0f));

        // Appliquer les transformations
        ComponentTransform* obstacleTransform = obstacle->GetComponent<ComponentTransform>();
        obstacleTransform->SetPosition(XMFLOAT3(posX, posY, posZ));
        obstacleTransform->SetScale(XMFLOAT3(scaleX, scaleY, scaleZ));
    }

    HEAD("OBSTACLES") << numObstacles << " murs generes aleatoirement dans la scene !" << ENDL;
}

void SystemScene::SpawnEnemies() {
    srand(static_cast<unsigned>(time(0))); // Initialisation de la seed aleatoire

    int numEnemies = 5 + (rand() % 6); // Genere entre 5 et 10 ennemis
    float levelSize = 50.0f; // Taille de la zone jouable

    for (int i = 0; i < numEnemies; i++) {
        // Generer une position aleatoire dans la zone de jeu
        float posX = (rand() % static_cast<int>(levelSize)) - (levelSize / 2);
        float posZ = (rand() % static_cast<int>(levelSize)) - (levelSize / 2);
        float posY = 0.0f; // Toujours sur le sol

        // Generer un type d'ennemi aleatoire (0 = normal, 1 = elite, 2 = mini-boss)
        int enemyType = rand() % 4;

        float scale, health, speed, dmg, cooldown;
        if (enemyType == 0 || enemyType == 1) { // Ennemi normal
            scale = 0.8f;
            health = 50.0f;
            speed = 1.2f;
			dmg = 10.0f;
			cooldown = 5.0f;
        }
        else if (enemyType == 2) { // Ennemi elite (plus rapide, un peu plus resistant)
            scale = 1.2f;
            health = 100.0f;
            speed = 1.7f;
			dmg = 15.0f;
			cooldown = 3.0f;
        }
        else { // Mini-boss (plus lent mais resistant)
            scale = 2.0f;
            health = 150.0f;
            speed = 0.8f;
			dmg = 20.0f;
			cooldown = 10.0f;

        }

        // Creation de l'ennemi
        Entity* enemy = GameEngine::GetInstance().GetEntityManager().CreateEntity();
        enemy->AddComponent<ComponentTransform>();
        enemy->AddComponent<ComponentRender>();
        enemy->AddComponent<ComponentCollider>(ColliderType::SPHERE, XMFLOAT3{ scale, scale * 2, scale });
        enemy->AddComponent<ComponentMesh>(GameEngine::GetInstance().GetWindow()->GetDevice(), GeometryGenerator::GenerateCylinder(0.5f, 1.0f, 10)); //GeometryGenerator::GenerateGeometryFromFile("../../../res/car.txt")
        enemy->AddComponent<ComponentPhysics>();
        enemy->AddComponent<ComponentHealth>(health, dmg); 
		enemy->AddComponent<ComponentAI>(speed, 1.0f, cooldown);

        // Appliquer les transformations
        ComponentTransform* enemyTransform = enemy->GetComponent<ComponentTransform>();
        enemyTransform->SetPosition(XMFLOAT3(posX, posY, posZ));
        enemyTransform->SetScale(XMFLOAT3(scale, scale * 2, scale));

        ComponentPhysics* enemyPhysics = enemy->GetComponent<ComponentPhysics>();
        enemyPhysics->SetaffectedByGravity(false);

    }

    HEAD("ENEMIES") << numEnemies << " ennemis generes avec difficulte aleatoire !" << ENDL;
}

void SystemScene::CreateCrosshair() {
    Entity* crosshair = GameEngine::GetInstance().GetEntityManager().CreateEntity();
    crosshair->AddComponent<ComponentTransform>();
    crosshair->AddComponent<ComponentRender>();

    // Stocker l'ID du viseur
    crosshairID = crosshair->GetID();

    // Position du viseur (on l'ajuste dynamiquement apres)
    ComponentTransform* transform = crosshair->GetComponent<ComponentTransform>();
    transform->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
    transform->SetScale(XMFLOAT3(0.02f, 0.02f, 0.02f)); // Taille ajustable

    // Definition d'un simple carre blanc comme viseur
    crosshair->AddComponent<ComponentMesh>(
        GameEngine::GetInstance().GetWindow()->GetDevice(),
        GeometryGenerator::GeneratePlane(0.1f, 0.1f)
    );

    ComponentRender* render = crosshair->GetComponent<ComponentRender>();
    render->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)); // Blanc
}



// Deplacement de la skybox ici
void SystemScene::CreateSkybox() {
    Entity* skybox = GameEngine::GetInstance().GetEntityManager().CreateEntity();
    skybox->AddComponent<ComponentTransform>();
    skybox->AddComponent<ComponentRender>();
    skybox->AddComponent<ComponentMesh>(GameEngine::GetInstance().GetWindow()->GetDevice(), GeometryGenerator::GenerateSkybox(100.0f));

    ComponentTransform* skyboxTransform = skybox->GetComponent<ComponentTransform>();
    skyboxTransform->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));

    // Enregistre la skybox dans le moteur
    GameEngine::GetInstance().SetSkyboxEntity(skybox);
}

void SystemScene::Update(float dt, const std::vector<Entity*>& entities) {
    // Plus tard, tu pourras ajouter du respawn ici
}


