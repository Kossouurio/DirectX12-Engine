#pragma once
#include <unordered_map>
//#include "Component.h"
//#include "UtilsImport.h"
using namespace DirectX;

struct Texture
{
    // Unique material name for lookup.
    std::string Name;

    std::wstring Path;

    ID3D12Resource* Resource = nullptr;
    ID3D12Resource* UploadHeap = nullptr;
};

class ComponentRender : public Component {
private:
    XMFLOAT4 color;
    Texture* texture;
    bool visible;

    std::unordered_map<Mesh*, ID3D12Resource*> constantBuffers;

public:
    ComponentRender(Entity* entity);
    
	void InitializeConstantBuffer(Mesh* mesh);

	void Logs() override;

    void SetColor(const XMFLOAT4& newColor);
	XMFLOAT4 GetColor() const;

    void SetTexture(Texture* _texture);
    Texture* GetTexture();

    void SetVisibility(bool isVisible);
	bool GetVisibility() const;

    void Render(float dt);
    void RenderParticle(float dt);
    Mesh* GetParticleMesh(ID3D12Device* device);
};
