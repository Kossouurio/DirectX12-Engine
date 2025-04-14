#include "pch.h"
#include "UtilsMain.h"

#include "D3D12Window.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow){

    D3D12Window d3d12Window;

    d3d12Window.InitWindow(hInstance);

    d3d12Window.InitD3D12();    

    MSG msg = { 0 };

    Logs::Get()->Init();
    HEAD("SYSTEM") << "====================== DEMARRAGE DU MOTEUR ECS ======================" << ENDL;

    // Creation du gestionnaire de systemes
    SystemManager systemManager;
    ScriptManager scriptManager;
    HEAD("SYSTEM") << "Gestionnaire de systemes initialise" << ENDL;

    // Creation des systemes
    SystemTransform* systemTransform = new SystemTransform();
    SystemPhysics* systemPhysics = new SystemPhysics();
    SystemJump* systemJump = new SystemJump();
    SystemRender* systemRender = new SystemRender();
    SystemCollider* systemCollider = new SystemCollider();
    SystemHealth* systemHealth = new SystemHealth();
    SystemInput* systemInput = new SystemInput();
    SystemCamera* systemCamera = new SystemCamera();
    HEAD("SYSTEM") << "Tous les systemes ECS sont crees" << ENDL;

    // Ajout des systemes au gestionnaire
    systemManager.AddSystem(systemTransform);
    systemManager.AddSystem(systemPhysics);
    systemManager.AddSystem(systemJump);
    systemManager.AddSystem(systemRender);
    systemManager.AddSystem(systemCollider);
    systemManager.AddSystem(systemHealth);
    systemManager.AddSystem(systemInput);
    systemManager.AddSystem(systemCamera);
    HEAD("SYSTEM") << "Systemes ajoutes au gestionnaire" << ENDL;

    // Creation du joueur et d'un ennemi
    Entity* player = new Entity(1);
    Entity* enemy = new Entity(2);
	Entity* camera = new Entity(99);
    HEAD("ENTITY") << "Creation des entites Joueur et Ennemi" << ENDL;

    // Ajout des composants au joueur
    ComponentTransform* playerTransform = player->AddComponent<ComponentTransform>();
    playerTransform->vPosition.y = 1.0f; // Met le joueur en hauteur au depart
    ComponentRender* playerRender = player->AddComponent<ComponentRender>();
    ComponentCollider* playerCollider = player->AddComponent<ComponentCollider>(ColliderType::AABB, XMFLOAT3{ 1.0f, 1.0f, 1.0f });
    ComponentHealth* playerHealth = player->AddComponent<ComponentHealth>(100, 5);
    ComponentInput* playerInput = player->AddComponent<ComponentInput>();

    // Ajout des composants a l'ennemi
    ComponentTransform* enemyTransform = enemy->AddComponent<ComponentTransform>();
    enemyTransform->vPosition.x = 5.0f; // Position initiale de l'ennemi
    ComponentRender* enemyRender = enemy->AddComponent<ComponentRender>();
    ComponentCollider* enemyCollider = enemy->AddComponent<ComponentCollider>(ColliderType::AABB, XMFLOAT3{ 1.0f, 1.0f, 1.0f });
    ComponentHealth* enemyHealth = enemy->AddComponent<ComponentHealth>(100, 10);

    // Ajout de composant a la camera
    ComponentCamera* cameraComponent = camera->AddComponent<ComponentCamera>();

    HEAD("COMPONENT") << "Composants ajoutes aux entites" << ENDL;

    // Ajout des entites aux systemes
    systemTransform->AddEntity(player);
    systemPhysics->AddEntity(player);
    systemJump->AddEntity(player);
    systemRender->AddEntity(player);
    systemCollider->AddEntity(player);
    systemHealth->AddEntity(player);
    systemInput->AddEntity(player);

    systemTransform->AddEntity(enemy);
    systemPhysics->AddEntity(enemy);
    systemRender->AddEntity(enemy);
    systemCollider->AddEntity(enemy);
    systemHealth->AddEntity(enemy);

    systemCamera->AddEntity(camera);

    HEAD("SYSTEM") << "Entites enregistrees dans les systemes" << "\n\n";

    // Simulation de la boucle de jeu avec entrees clavier
    HEAD("SYSTEM") << "====================== DEBUT DE LA BOUCLE DE JEU ======================" << ENDL;

    float dt = 0.016f; // Simulation de 60 FPS
    int i = 0;

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            d3d12Window.Draw(0.0f);

            i++;
            if (i % 10 == 0)
            {
                HEAD("FRAME") << "Mise a jour ECS - Frame " << i << ENDL;

               // Gestion des entrées clavier 
                if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                    Input::SetKeyPressed(KEY_LEFT, true);
                }
                else {
                    Input::SetKeyPressed(KEY_LEFT, false);
                }

                if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                    Input::SetKeyPressed(KEY_RIGHT, true);
                }
                else {
                    Input::SetKeyPressed(KEY_RIGHT, false);
                }

                if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                    Input::SetKeyPressed(KEY_SPACE, true);
                }
                else {
                    Input::SetKeyPressed(KEY_SPACE, false);
                }

                if (GetAsyncKeyState(VK_UP) & 0x8000) {
                    Input::SetKeyPressed(KEY_UP, true);
                }
                else {
                    Input::SetKeyPressed(KEY_UP, false);
                }

                if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                    Input::SetKeyPressed(KEY_DOWN, true);
                }
                else {
                    Input::SetKeyPressed(KEY_DOWN, false);
                }

                if (GetAsyncKeyState(0x5A) & 0x8000) {
                    Input::SetKeyPressed(KEY_Z, true);
                }
                else {
                    Input::SetKeyPressed(KEY_Z, false);
                }

                if (GetAsyncKeyState(0x51) & 0x8000) { 
                    Input::SetKeyPressed(KEY_Q, true);
                }
                else {
                    Input::SetKeyPressed(KEY_Q, false);
                }

                if (GetAsyncKeyState(0x53) & 0x8000) {
                    Input::SetKeyPressed(KEY_S, true);
                }
                else {
                    Input::SetKeyPressed(KEY_S, false);
                }

                if (GetAsyncKeyState(0x44) & 0x8000) { 
                    Input::SetKeyPressed(KEY_D, true);
                }
                else {
                    Input::SetKeyPressed(KEY_D, false);
                }



                // Mise a jour du moteur et des scripts
                scriptManager.Update(dt);
                systemManager.Update(dt);

                // Verification des positions et vitesses
                HEAD("TRANSFORM") << "Joueur Position: (" << playerTransform->vPosition.x << ", "
                    << playerTransform->vPosition.y << ", " << playerTransform->vPosition.z << ")" << ENDL;

                HEAD("TRANSFORM") << "Ennemi Position: (" << enemyTransform->vPosition.x << ", "
                    << enemyTransform->vPosition.y << ", " << enemyTransform->vPosition.z << ")" << ENDL;

                // Verification de la vie
                HEAD("HEALTH") << "Joueur HP: " << playerHealth->GetHealth() << ENDL;
                HEAD("HEALTH") << "Ennemi HP: " << enemyHealth->GetHealth() << ENDL;

                // Verification des collisions
                if (playerCollider->CheckCollision(enemyCollider)) {
                    HEAD("COLLISION") << "Le Joueur a percute l'Ennemi !" << ENDL;
                }

                // Suppression si l'entite meurt
                if (playerHealth->GetHealth() <= 0) {
                    HEAD("HEALTH") << "Le Joueur est mort !" << ENDL;
                    delete player;
                    player = nullptr;
                }
                if (enemyHealth->GetHealth() <= 0) {
                    HEAD("HEALTH") << "L'Ennemi est mort !" << ENDL;
                    delete enemy;
                    enemy = nullptr;
                }

                if (!player && !enemy) {
                    HEAD("SYSTEM") << "Toutes les entites sont mortes, fin du jeu." << ENDL;
                    break;
                }

                // Reinitialisation des entrees clavier
                Input::SetKeyPressed(KEY_LEFT, false);
                Input::SetKeyPressed(KEY_RIGHT, false);
                Input::SetKeyPressed(KEY_SPACE, false);

                HEAD("FRAME") << "Fin de la frame " << i << "\n\n";
            }
        }
        
    }

    HEAD("SYSTEM") << "====================== FIN DE LA BOUCLE DE JEU ======================" << ENDL;

    // Nettoyage memoire
    if (player) delete player;
    if (enemy) delete enemy;

    HEAD("SYSTEM") << "Fermeture du moteur ECS" << ENDL;
    Logs::Get()->UnInit();

    d3d12Window.CleanUp();
    return static_cast<int>(msg.wParam);
}
