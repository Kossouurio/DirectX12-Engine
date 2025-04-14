#pragma once


class GameEngine {
public:
    static GameEngine& GetInstance();  // Singleton
    EntityManager& GetEntityManager(); 
	SystemManager& GetSystemManager() {
		return systemManager;
	}

    GameTimer& GetTimer();

	Camera* GetCamera();

	ComponentCamera* GetMainCamera() {
		return player->GetComponent<ComponentCamera>();
	}

	D3D12Window* GetWindow() {
		return &d3d12Window;
	}

    SystemParticle* GetParticleSystem() {
        return systemManager.GetSystem<SystemParticle>();
    }

    Entity* GetPlayerEntity();
    Entity* Getplayer();

    void SetPlayerEntity(Entity* entity) { player = entity; }
    void SetSkyboxEntity(Entity* entity) { skybox = entity; }

    void Init(HINSTANCE hInstance);
    void CreateStarfield(int numStars = 400);
    void UpdateStarfield(float dt);
    void Run();
    void CleanUp();
    void CreateStartButton();
    bool IsGameStarted() const { return m_gameStarted; }
    void StartGame();
    bool IsCameraLocked() const { return !m_gameStarted; }
    void CreateHealthBar();
    void UpdateHealthBarPosition();
    void TestHealthBarDecrease();

private:
    GameTimer gameTimer;

    D3D12Window d3d12Window;
    SystemManager systemManager;
    ScriptManager scriptManager;
    EntityManager entityManager;  

    Entity* player;
    Entity* enemy;
	Entity* skybox;
    Entity* m_healthBar = nullptr;
    Entity* m_startButton;
    bool m_gameStarted = false;



    std::vector<Entity*> m_stars;
    bool m_starfieldCreated = false;

    void InitWindow(HINSTANCE hInstance);
    void InitSystems();
    void InitEntities();
	//void RegisterEntitiesToSystems();
    void GameLoop();

	void RotateAnimation(float dt, std::vector<Entity*> entities);
	void UpdateSkybox(float dt);
};



