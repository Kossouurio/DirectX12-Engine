#include "pch.h"

GameEngine& GameEngine::GetInstance() {
    static GameEngine instance;
    return instance;
}

EntityManager& GameEngine::GetEntityManager() {
    return entityManager;
}

GameTimer& GameEngine::GetTimer() {
    return gameTimer;
}

Camera* GameEngine::GetCamera() {
	ComponentCamera* cameraComponent = player->GetComponent<ComponentCamera>();
	if (cameraComponent) {
		return cameraComponent->GetCamera();
	}
	return nullptr;
}

Entity* GameEngine::GetPlayerEntity() {
    return player;
}

Entity* GameEngine::Getplayer() {
    return player;
}

void GameEngine::Init(HINSTANCE hInstance) {
    Logs::Get()->Init();
    HEAD("CLEAN") << "====================== DEMARRAGE DU MOTEUR ECS ======================" << ENDL;

    InitWindow(hInstance);
    InitSystems();

    // Initialisation du mode Raw Input pour la souris
    HWND hwnd = d3d12Window.GetHwnd();  // Assure-toi que cette fonction existe
    Input::EnableRawMouse(hwnd);

    // Verification des buffers AVANT l'initialisation des particules
    SystemParticle* particleSystem = systemManager.GetSystem<SystemParticle>();
    if (particleSystem && d3d12Window.GetBuffers()) {
        HEAD("SETUP") << "Initialisation complete du systeme de particules avec buffers valides." << ENDL;
        particleSystem->Initialize(d3d12Window.GetDevice(), d3d12Window.GetBuffers());
    }
    else {
        HEAD("ERROR") << "Impossible d'initialiser SystemParticle, buffers non disponibles !" << ENDL;
    }

    InitEntities();  // Les entites sont bien creees APRES l'initialisation des systemes
    CreateStarfield(400);
    CreateStartButton();
    CreateHealthBar();
    HEAD("CLEAN") << "====================== INITIALISATION TERMINEE ======================" << ENDL;
}

void GameEngine::CreateStarfield(int numStars) {
    HEAD("STARFIELD") << "Cr�ation d'un champ d'�toiles avec " << numStars << " �toiles" << ENDL;

   
    const float starDistance = 95.0f; 

    
    for (int i = 0; i < numStars; i++) {
        Entity* star = entityManager.CreateEntity();

       
        ComponentTransform* transform = star->AddComponent<ComponentTransform>();
        ComponentRender* render = star->AddComponent<ComponentRender>();
        ParticleComponent* particle = star->AddComponent<ParticleComponent>();
		particle->SetGeometryType(ParticleGeometryType::PLANE); 


        
        float theta = ((float)rand() / RAND_MAX) * XM_2PI; 
        float phi = acos(2.0f * ((float)rand() / RAND_MAX) - 1.0f); 

       
        XMFLOAT3 position(
            starDistance * sin(phi) * cos(theta),
            starDistance * sin(phi) * sin(theta),
            starDistance * cos(phi)
        );

        transform->SetPosition(position);

        
        float size = 0.2f + ((float)rand() / RAND_MAX) * 0.3f; 
        transform->SetScale(XMFLOAT3(size, size, size));

        
        render->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

       
        particle->SetLifeTime(10000.0f, 10000.0f);
        particle->SetActive(true);

        
        m_stars.push_back(star);
    }

    m_starfieldCreated = true;
    HEAD("STARFIELD") << "Champ d'�toiles cr�� avec succ�s" << ENDL;
}

void GameEngine::UpdateStarfield(float dt) {
    if (!m_starfieldCreated) return;

    
    ComponentCamera* camera = GetMainCamera();
    if (!camera) return;
    Camera* cam = camera->GetCamera();

    
    XMMATRIX viewMatrix = cam->GetViewMatrix();
    XMMATRIX projMatrix = cam->GetProjectionMatrix();
    XMMATRIX viewProjMatrix = XMMatrixMultiply(viewMatrix, projMatrix);

    
    XMFLOAT3 cameraPos = cam->GetPosition();

   
    int visibleStars = 0;

    
    for (Entity* star : m_stars) {
        ComponentTransform* transform = star->GetComponent<ComponentTransform>();
        ComponentRender* render = star->GetComponent<ComponentRender>();
        ParticleComponent* particle = star->GetComponent<ParticleComponent>();

        if (!transform || !render || !particle) continue;

        XMFLOAT3 starPos = transform->GetPosition();
        XMVECTOR posVector = XMLoadFloat3(&starPos);

        XMVECTOR projectedPos = XMVector3TransformCoord(posVector, viewProjMatrix);

        XMFLOAT3 projectedCoords;
        XMStoreFloat3(&projectedCoords, projectedPos);

        float margin = 0.2f; 

        bool inFrustumX = (projectedCoords.x >= -1.0f - margin) && (projectedCoords.x <= 1.0f + margin);
        bool inFrustumY = (projectedCoords.y >= -1.0f - margin) && (projectedCoords.y <= 1.0f + margin);
        bool inFrustumZ = (projectedCoords.z >= 0.0f) && (projectedCoords.z <= 1.0f);

        XMVECTOR toStar = XMVectorSubtract(posVector, XMLoadFloat3(&cameraPos));
        XMVECTOR dirToStar = XMVector3Normalize(toStar);
        XMVECTOR forward = cam->GetForward();
        float dotProduct;
        XMStoreFloat(&dotProduct, XMVector3Dot(dirToStar, forward));

        if (inFrustumX && inFrustumY && inFrustumZ && dotProduct > 0.0f) {
            
            render->SetVisibility(true);
            particle->SetActive(true);

            render->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

            if (particle->GetGeometryType() == ParticleGeometryType::PLANE) {
              
                XMVECTOR camRight = cam->GetRight();
                XMVECTOR camUp = cam->GetUp();
                XMVECTOR camForward = cam->GetForward();

                
                XMFLOAT3 right, up, forward;
                XMStoreFloat3(&right, camRight);
                XMStoreFloat3(&up, camUp);
                XMStoreFloat3(&forward, camForward);

                
                transform->SetRight(right);
                transform->SetUp(up);
                transform->SetForward(forward);
                transform->UpdateVectors();

                
                if (visibleStars == 0) {
                    HEAD("PLANE DEBUG") << "Using camera orientation directly" << ENDL;
                    HEAD("PLANE DEBUG") << "Camera Right: (" << right.x << ", " << right.y << ", " << right.z << ")" << ENDL;
                    HEAD("PLANE DEBUG") << "Camera Up: (" << up.x << ", " << up.y << ", " << up.z << ")" << ENDL;
                    HEAD("PLANE DEBUG") << "Camera Forward: (" << forward.x << ", " << forward.y << ", " << forward.z << ")" << ENDL;
                }
            }

            visibleStars++;
        }
        else {
           
            render->SetVisibility(false);
            particle->SetActive(false);
        }
    }

   
    HEAD("STARFIELD") << "�toiles visibles: " << visibleStars << "/" << int(m_stars.size()) << ENDL;
}

void GameEngine::InitWindow(HINSTANCE hInstance) {
    d3d12Window.InitWindow(hInstance);
    d3d12Window.InitD3D12();
}


void GameEngine::CreateStartButton() {
    HEAD("UI") << "Création du bouton de démarrage (Appuyez sur 'E' pour commencer)" << ENDL;

    m_startButton = entityManager.CreateEntity();

    m_startButton->AddComponent<ComponentTransform>();
    m_startButton->AddComponent<ComponentRender>();
    m_startButton->AddComponent<ComponentMesh>(d3d12Window.GetDevice(), GeometryGenerator::GenerateCube(1.0f));

    ComponentTransform* buttonTransform = m_startButton->GetComponent<ComponentTransform>();
    
    buttonTransform->SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));
    buttonTransform->SetScale(XMFLOAT3(2.0f, 0.7f, 0.1f));

    ComponentMesh* buttonMesh = m_startButton->GetComponent<ComponentMesh>();
    buttonMesh->GetMesh()->SetColor(0.2f, 0.5f, 0.8f);

    ComponentCamera* camera = GetMainCamera();
    if (camera) {
        ComponentTransform* camTransform = player->GetComponent<ComponentTransform>();
        if (camTransform) {
            camTransform->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f)); 
        }
    }

    HEAD("UI") << "Bouton de démarrage créé - Appuyez sur 'E' pour commencer le jeu" << ENDL;
    HEAD("UI") << "Caméra verrouillée jusqu'au démarrage du jeu" << ENDL;

}

void GameEngine::CreateHealthBar() {
    HEAD("UI") << "Création de la barre de vie fixée à l'écran" << ENDL;

  
    Entity* healthBar = entityManager.CreateEntity();


    healthBar->AddComponent<ComponentTransform>();
    healthBar->AddComponent<ComponentRender>();
    healthBar->AddComponent<ComponentMesh>(d3d12Window.GetDevice(), GeometryGenerator::GenerateCube(1.0f));
    healthBar->AddComponent<ComponentUI>();

    
    if (!player->GetComponent<ComponentHealth>()) {
        player->AddComponent<ComponentHealth>(100, 10);
    }

   
    ComponentTransform* barTransform = healthBar->GetComponent<ComponentTransform>();

 
    barTransform->SetScale(XMFLOAT3(0.4f, 0.05f, 0.01f));

    
    ComponentMesh* barMesh = healthBar->GetComponent<ComponentMesh>();
    barMesh->GetMesh()->SetColor(0.0f, 0.8f, 0.0f); 

  
    ComponentUI* barUI = healthBar->GetComponent<ComponentUI>();
    barUI->SetType(UIElementType::LIFE);
    barUI->SetVisible(true);
    barUI->SetClickable(false);
    barUI->SetFillColor(XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f)); 

  
    ComponentHealth* playerHealth = player->GetComponent<ComponentHealth>();
    barUI->LinkToHealth(playerHealth);

 
    m_healthBar = healthBar;

    HEAD("UI") << "Barre de vie créée avec succès" << ENDL;
}

// Dans GameEngine.cpp
void GameEngine::UpdateHealthBarPosition() {
    if (!m_healthBar) return;

    ComponentTransform* barTransform = m_healthBar->GetComponent<ComponentTransform>();
    if (!barTransform) return;

  
    D3D12Window* window = GetWindow();
    if (!window) return;

 

    float screenX = window->GetWindowWidth() *0.1f;
    float screenY = window->GetWindowHeight() * 0.9f;


    ComponentCamera* camera = GetMainCamera();
    if (!camera) return;

    Camera* cam = camera->GetCamera();
    if (!cam) return;


    XMMATRIX viewMatrix = cam->GetViewMatrix();
    XMMATRIX projMatrix = cam->GetProjectionMatrix();

  
    XMMATRIX viewportMatrix = XMMatrixSet(
        window->GetWindowWidth() / 2.0f, 0, 0, 0,
        0, -window->GetWindowHeight() / 2.0f, 0, 0,
        0, 0, 1, 0,
        window->GetWindowWidth() / 2.0f, window->GetWindowHeight() / 2.0f, 0, 1
    );


    XMMATRIX invViewportMatrix = XMMatrixInverse(nullptr, viewportMatrix);
    XMMATRIX invProjMatrix = XMMatrixInverse(nullptr, projMatrix);
    XMMATRIX invViewMatrix = XMMatrixInverse(nullptr, viewMatrix);


    XMVECTOR screenPoint = XMVectorSet(screenX, screenY, 0.1f, 1.0f);

    XMVECTOR projPoint = XMVector4Transform(screenPoint, invViewportMatrix);

    XMVECTOR camPoint = XMVector4Transform(projPoint, invProjMatrix);
    camPoint = XMVectorScale(camPoint, 1.0f / XMVectorGetW(camPoint));

    XMVECTOR worldPoint = XMVector4Transform(camPoint, invViewMatrix);

    XMFLOAT3 worldPos;
    XMStoreFloat3(&worldPos, worldPoint);


    barTransform->SetPosition(worldPos);

    XMFLOAT3 cameraPos = cam->GetPosition();
    XMFLOAT3 toCamera;
    toCamera.x = cameraPos.x - worldPos.x;
    toCamera.y = cameraPos.y - worldPos.y;
    toCamera.z = cameraPos.z - worldPos.z;

  
    float length = sqrt(toCamera.x * toCamera.x + toCamera.y * toCamera.y + toCamera.z * toCamera.z);
    if (length > 0.001f) {
        toCamera.x /= length;
        toCamera.y /= length;
        toCamera.z /= length;
    }

    barTransform->SetForward(toCamera);

    XMVECTOR forwardVector = XMLoadFloat3(&toCamera);
    XMVECTOR worldUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


    if (fabs(XMVectorGetX(XMVector3Dot(forwardVector, worldUp))) > 0.99f) {
        worldUp = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    }

    
    XMVECTOR rightVector = XMVector3Cross(worldUp, forwardVector);
    rightVector = XMVector3Normalize(rightVector);

    XMVECTOR upVector = XMVector3Cross(forwardVector, rightVector);

    XMFLOAT3 rightFloat, upFloat;
    XMStoreFloat3(&rightFloat, rightVector);
    XMStoreFloat3(&upFloat, upVector);

    barTransform->SetRight(rightFloat);
    barTransform->SetUp(upFloat);
    barTransform->UpdateVectors();


    static int frameCount = 0;
    if (frameCount++ % 300 == 0) {
        HEAD("HEALTH_BAR") << "Screen Position: (" << screenX << ", " << screenY << ")" << ENDL;
        HEAD("HEALTH_BAR") << "World Position: (" << worldPos.x << ", "
            << worldPos.y << ", " << worldPos.z << ")" << ENDL;
    }
}

void GameEngine::TestHealthBarDecrease() {
    ComponentHealth* playerHealth = player->GetComponent<ComponentHealth>();
    if (playerHealth) {
        int currentHealth = playerHealth->GetHealth();
        if (currentHealth > 0) {
            playerHealth->TakeDamage(5); 
            HEAD("HEALTH") << "Vie du joueur: " << playerHealth->GetHealth() << "/"
                << playerHealth->GetMaxHealth() << ENDL;
        }
    }
}

void GameEngine::StartGame() {
    m_gameStarted = true;

    if (m_startButton) {
        entityManager.DestroyEntity(m_startButton->GetID());
        m_startButton = nullptr;
        HEAD("UI") << "Bouton de démarrage supprimé" << ENDL;
    }
    
    

    HEAD("GAME") << "Le jeu commence !" << ENDL;
    HEAD("UI") << "Caméra déverrouillée" << ENDL;
}

void GameEngine::InitSystems() {
    HEAD("SETUP") << "Gestionnaire de systemes initialise" << ENDL;

    systemManager.AddSystem<SystemTransform>();
    systemManager.AddSystem<SystemPhysics>();
	systemManager.AddSystem<SystemInput>();
    systemManager.AddSystem<SystemRender>();
    systemManager.AddSystem<SystemCollider>();
    systemManager.AddSystem<SystemCamera>();
	systemManager.AddSystem<SystemParticle>();
    systemManager.AddSystem<SystemUI>();
	systemManager.AddSystem<SystemGameplay>();
    systemManager.AddSystem<SystemScene>();
	systemManager.AddSystem<SystemAI>();



    HEAD("SETUP") << "Systemes ajoutes au gestionnaire" << ENDL;

    SystemParticle* particleSystem = systemManager.GetSystem<SystemParticle>();
    if (particleSystem && !particleSystem->isInitialized()) {
        particleSystem->Initialize(d3d12Window.GetDevice(), d3d12Window.GetBuffers());
    }

    SystemGameplay* gameplaySystem = systemManager.GetSystem<SystemGameplay>();
    SystemInput* inputSystem = systemManager.GetSystem<SystemInput>();

    if (inputSystem && gameplaySystem) {
        inputSystem->SetGameplaySystem(gameplaySystem);
    }

}


void GameEngine::InitEntities() {
    SystemScene* sceneSystem = systemManager.GetSystem<SystemScene>();
    if (sceneSystem) {
        sceneSystem->InitScene();
        sceneSystem->CreateCrosshair();
    }

    // Mise a jour des colliders apres creation des entites
    for (auto* entity : entityManager.GetAllEntities()) {
        ComponentMesh* mesh = entity->GetComponent<ComponentMesh>();
        ComponentCollider* collider = entity->GetComponent<ComponentCollider>();

        if (mesh && collider) {
            collider->UpdateColliderSize();
        }
    }

    HEAD("SETUP") << "Entites principales chargees !" << ENDL;
}


void GameEngine::GameLoop() {
    MSG msg = { 0 };
    int frameCount = 0;

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            gameTimer.Tick();
            float dt = gameTimer.DeltaTime();

            d3d12Window.Draw(0.0f);

            if (logFrame) {
                HEAD("FRAME") << "Mise � jour ECS - Frame " << frameCount++ << ENDL;
            }

            Input::CheckInput();

            if (m_gameStarted && Input::IsKeyPressed('S')) {
                TestHealthBarDecrease(); // Diminuer la vie en appuyant sur H
            }

			if (Input::IsKeyPressed('P')) {
                d3d12Window.DefaultLights = false;
			}

            UpdateStarfield(dt);
			UpdateHealthBarPosition();
            if (!m_gameStarted && Input::IsKeyPressed('E')) {
                StartGame();
                HEAD("INPUT") << "Touche E détectée!" << ENDL;
            }

            if (!m_gameStarted) {
               
                ComponentTransform* camTransform = player->GetComponent<ComponentTransform>();
                if (camTransform) {
                    camTransform->SetPosition(XMFLOAT3(0.0f, 0.0f, -10.0f));
                }
            }
            
           
            systemManager.Update(dt);

           
            d3d12Window.Draw(dt);

            Input::ResetInput();
            //RotateAnimation(dt, entityManager.GetAllEntities());
            //UpdateSkybox(dt);

            if (logFrame) {
                HEAD("FRAME") << LINE << ENDL2;
            }
        }
    }
    HEAD("CLEAN") << "====================== FIN DE LA BOUCLE DE JEU ======================" << ENDL;
}





void GameEngine::Run() {
    GameLoop();
}

void GameEngine::CleanUp() {
	m_stars.clear();
    delete player;
    delete enemy;
    Logs::Get()->UnInit();
    d3d12Window.CleanUp();
    HEAD("CLEAN") << "Fermeture du moteur ECS" << ENDL;
}

void GameEngine::RotateAnimation(float dt, std::vector<Entity*> entities) {

    for (auto* entity : entities) {
		// sauf pour la camera et le skybox
		if (entity == player || entity == skybox) {
			continue;
		}

        ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
        ComponentMesh* mesh = entity->GetComponent<ComponentMesh>();

        if (transform && mesh) {
            int entityID = entity->GetID();
            float rotationSpeed = XMConvertToRadians(20.0f + (entityID % 50)) * dt;

            XMFLOAT3 rotationAxis = XMFLOAT3(
                ((entityID % 3) - 1) * 1.0f,  
                1.0f,  
                ((entityID % 2) * 2 - 1) * 1.0f
            );

            transform->RotateAroundAxis(rotationAxis, rotationSpeed);
        }
    }

}

void GameEngine::UpdateSkybox(float dt)
{
	// Set la position de la skybox a la position de la camera
	ComponentTransform* cameraTransform = player->GetComponent<ComponentTransform>();
	ComponentTransform* skyboxTransform = skybox->GetComponent<ComponentTransform>();
	skyboxTransform->SetPosition(cameraTransform->GetPosition());
}
