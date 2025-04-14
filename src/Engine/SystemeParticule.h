#pragma once

// Structure de donnees pour une particule dans le shader
struct ParticleVertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT4 color;
    float size;
    float life;
};

class SystemParticle : public System {
private:
    bool m_initialized;
    D3D12Buffers* m_buffers;
    std::vector<ParticleVertex> m_activeParticles;

public:
    SystemParticle();
    ~SystemParticle();

    void Initialize(ID3D12Device* device, D3D12Buffers* buffers);
    void Update(float dt, const std::vector<Entity*>& entities) override;

    bool isInitialized() const { return m_initialized; }
};