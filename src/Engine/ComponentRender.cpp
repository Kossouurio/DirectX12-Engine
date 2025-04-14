#include "pch.h"
#include "ComponentRender.h"

ComponentRender::ComponentRender(Entity* entity)
    : Component(entity), color({ 1.0f, 1.0f, 1.0f, 1.0f }), visible(true)
{
}

void ComponentRender::InitializeConstantBuffer(Mesh* mesh) {
    auto device = GameEngine::GetInstance().GetWindow()->GetDevice();
    auto bufferSize = sizeof(ConstantBufferData);

    // Creer le buffer de constantes
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
    ID3D12Resource* constantBuffer;

    HRESULT hr = device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constantBuffer)
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create constant buffer.");
    }

    // Stocker le buffer de constantes
    constantBuffers[mesh] = constantBuffer;
}

void ComponentRender::Logs() {
    HEAD("sys_RENDER") << "Rendering Entity " << GetEntity()->GetID()
        << " | Color: (" << GetColor().x << ", "
        << GetColor().y << ", " << GetColor().z << ")" << ENDL;
}

void ComponentRender::SetColor(const XMFLOAT4& newColor) 
{
    color = newColor;
}

XMFLOAT4 ComponentRender::GetColor() const
{
	return color;
}

void ComponentRender::SetTexture(Texture* _texture)
{
	texture = _texture;
}

Texture* ComponentRender::GetTexture()
{
    return texture;
}


void ComponentRender::SetVisibility(bool isVisible) 
{
    visible = isVisible;
}

bool ComponentRender::GetVisibility() const
{
	return visible;
}

void ComponentRender::Render(float dt)
{
    if (!GetEntity()) {
        HEAD("ERROR") << "ComponentRender::Render() - GetEntity() est nullptr !" << ENDL;
        return;
    }

    ComponentMesh* meshComponent = GetEntity()->GetComponent<ComponentMesh>();
    if (!meshComponent) {
        ParticleComponent* particleComp = GetEntity()->GetComponent<ParticleComponent>();
        if (particleComp && particleComp->IsActive()) {
            RenderParticle(dt);
            return;
        }
        return;
    }

    Mesh* mesh = meshComponent->GetMesh();
    if (constantBuffers.find(mesh) == constantBuffers.end()) {
        InitializeConstantBuffer(mesh);
    }

    D3D12Window* renderer = GameEngine::GetInstance().GetWindow();
    ComponentCamera* cameraComponent = GameEngine::GetInstance().GetMainCamera();

    // Obtenir les matrices de transformation
    XMMATRIX worldMatrix = GetEntity()->GetComponent<ComponentTransform>()->GetWorldMatrix();
    XMMATRIX viewMatrix = cameraComponent->GetCamera()->GetViewMatrix();
    XMMATRIX projMatrix = cameraComponent->GetCamera()->GetProjectionMatrix();
    XMMATRIX wvpMatrix = worldMatrix * viewMatrix * projMatrix;

    // Mettre a jour le constant buffer
    //auto& cbData = renderer->GetCbData();
    ConstantBufferData cbData;
    cbData.World = XMMatrixTranspose(worldMatrix);
    cbData.View = XMMatrixTranspose(viewMatrix);
    cbData.Projection = XMMatrixTranspose(projMatrix);
    cbData.WorldViewProj = XMMatrixTranspose(wvpMatrix);

    static float totalTime = 0.0f;
    totalTime += dt;
    cbData.Time = XMFLOAT4(totalTime, dt, 0.0f, 0.0f);

    // Copier les donnees dans le buffer de constantes
    void* pData;
    constantBuffers[mesh]->Map(0, nullptr, &pData);
    memcpy(pData, &cbData, sizeof(cbData));
    constantBuffers[mesh]->Unmap(0, nullptr);

    // Mise a jour du buffer GPU
    renderer->GetCommandList()->SetGraphicsRootConstantBufferView(
        0,
        constantBuffers[mesh]->GetGPUVirtualAddress()
    );

	// Mise a jour de la texture
	//if (texture.Resource) {
	//	renderer->GetCommandList()->SetGraphicsRootDescriptorTable(2, texture.Resource->GetGPUVirtualAddress()); // Texture slot 2 (index 1) dans le root signature
	//}

    // Dessiner le mesh
    //meshComponent->GetMesh()->Draw(renderer->GetCommandList());
    mesh->Draw(renderer->GetCommandList());
}

void ComponentRender::RenderParticle(float dt)
{
    D3D12Window* renderer = GameEngine::GetInstance().GetWindow();
    ComponentCamera* cameraComponent = GameEngine::GetInstance().GetMainCamera();
    ComponentTransform* transform = GetEntity()->GetComponent<ComponentTransform>();
    ParticleComponent* particle = GetEntity()->GetComponent<ParticleComponent>();

    if (!transform || !particle || !particle->IsActive() || !visible) {
        return; // Ne pas dessiner si la particule n'est pas active ou visible
    }

    // Obtenir un mesh partage pour toutes les particules
    static Mesh* sharedCubeMesh = nullptr;
    static Mesh* sharedPlaneMesh = nullptr;
    Mesh* particleMesh = nullptr;
    if (particle->GetGeometryType() == ParticleGeometryType::CUBE) {
        // Creer et utiliser un cube
        if (!sharedCubeMesh) {
            Geometry cubeGeometry = GeometryGenerator::GenerateCube(1.0f);
            sharedCubeMesh = new Mesh(renderer->GetDevice());
            sharedCubeMesh->Initialize(cubeGeometry);
            sharedCubeMesh->SetColor(1.0f, 1.0f, 1.0f);
        }
        particleMesh = sharedCubeMesh;
    }
    else { // PLANE
        if (!sharedPlaneMesh) {
            Geometry planeGeometry = GeometryGenerator::GeneratePlane(1.0f, 1.0f);
            sharedPlaneMesh = new Mesh(renderer->GetDevice());
            sharedPlaneMesh->Initialize(planeGeometry);
            sharedPlaneMesh->SetColor(1.0f, 1.0f, 1.0f);
        }
        particleMesh = sharedPlaneMesh;
    }

    // Initialiser le buffer constant si necessaire
    if (constantBuffers.find(particleMesh) == constantBuffers.end()) {
        InitializeConstantBuffer(particleMesh);
    }

    // Position de la particule
    XMFLOAT3 position = transform->GetPosition();
    XMVECTOR particlePosition = XMLoadFloat3(&position);

    // Creer une matrice de billboard pour que la particule soit toujours face a la camera
    XMMATRIX viewMatrix = cameraComponent->GetCamera()->GetViewMatrix();
    XMMATRIX projMatrix = cameraComponent->GetCamera()->GetProjectionMatrix();

    // Extraire les axes droit et haut a partir de la matrice de vue
    XMVECTOR right = XMVectorSet(viewMatrix.r[0].m128_f32[0], viewMatrix.r[0].m128_f32[1], viewMatrix.r[0].m128_f32[2], 0.0f);
    XMVECTOR up = XMVectorSet(viewMatrix.r[1].m128_f32[0], viewMatrix.r[1].m128_f32[1], viewMatrix.r[1].m128_f32[2], 0.0f);

    // Inverser les vecteurs puisque la matrice de vue est l'inverse de la camera
    right = XMVectorNegate(right);

    // Creer la matrice de billboard
    XMMATRIX billboard;
    billboard.r[0] = right;
    billboard.r[1] = up;
    billboard.r[2] = XMVector3Cross(right, up);
    billboard.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    // Taille de la particule basee sur l'echelle du transform
    float scaleValue = transform->GetScale();
    XMMATRIX scaleMatrix = XMMatrixScaling(scaleValue, scaleValue, scaleValue);

    // Position de la particule
    XMMATRIX translationMatrix = XMMatrixTranslationFromVector(particlePosition);

    // Matrice finale pour la particule
    XMMATRIX worldMatrix = scaleMatrix * billboard * translationMatrix;

    // Calculer la matrice WorldViewProj
    XMMATRIX wvpMatrix = worldMatrix * viewMatrix * projMatrix;

    // Mettre a jour le buffer constant
    ConstantBufferData cbData;
    cbData.World = XMMatrixTranspose(worldMatrix);
    cbData.View = XMMatrixTranspose(viewMatrix);
    cbData.Projection = XMMatrixTranspose(projMatrix);
    cbData.WorldViewProj = XMMatrixTranspose(wvpMatrix);

    // Ajouter le temps pour les effets
    static float totalTime = 0.0f;
    totalTime += dt;
    cbData.Time = XMFLOAT4(totalTime, dt, 1.0f, 0.0f); // Toujours pleine intensite

    // Copier les donnees dans le buffer constant
    void* pData;

    constantBuffers[particleMesh]->Map(0, nullptr, &pData);
    memcpy(pData, &cbData, sizeof(cbData));
    constantBuffers[particleMesh]->Unmap(0, nullptr);

    // Utiliser le pipeline state specifique aux particules s'il existe
    ID3D12GraphicsCommandList* cmdList = renderer->GetCommandList();
    ID3D12PipelineState* particlePSO = renderer->GetParticlePipeline();
    if (particlePSO) {
        cmdList->SetPipelineState(particlePSO);
    }

    // Lier le buffer constant
    cmdList->SetGraphicsRootConstantBufferView(
        0,
        constantBuffers[particleMesh]->GetGPUVirtualAddress()
    );

    // Dessiner le mesh
    particleMesh->Draw(cmdList);
}


Mesh* ComponentRender::GetParticleMesh(ID3D12Device* device)
{
    static Mesh* particleMesh = nullptr;

    if (!particleMesh) {
        Geometry quadGeometry = GeometryGenerator::GenerateCube(10);
        particleMesh = new Mesh(device);
        particleMesh->Initialize(quadGeometry);
        particleMesh->SetColor(1.0f, 1.0f, 1.0f); // Couleur blanche de base
    }

    return particleMesh;
}