#pragma once

class D3D12Buffers {
public:
    D3D12Buffers(ID3D12Device* device);
    ~D3D12Buffers();

    // Initialisation des buffers
    void CreateVertexBuffer(const Geometry& geometry);
    void CreateIndexBuffer(const Geometry& geometry);
    void CreateConstantBuffers(UINT frameCount);

    // Accesseurs&
    const D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() const { return &mVertexBufferView; }
    const D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() const { return &mIndexBufferView; }
    ID3D12Resource* GetConstantBuffer(UINT frameIndex) const { return mConstantBuffers[frameIndex]; }
    
	// Creation du buffer de particules
    void CreateParticleBuffer(ID3D12Device* device, UINT maxParticles, UINT particleStride);
    void UpdateParticleBuffer(const void* data, UINT dataSize, ID3D12GraphicsCommandList* cmdList = nullptr);
    D3D12_VERTEX_BUFFER_VIEW GetParticleBufferView(UINT particleCount) const;

    // Getter pour le buffer de particules
    ID3D12Resource* GetParticleBuffer() const { return mParticleBuffer; }
    ID3D12Resource* GetParticleUploadBuffer() const { return mParticleUploadBuffer; }

    // Mise a jour des constantes
    template<typename T>
    void UpdateConstantBuffer(UINT frameIndex, const T& data);

private:
    ID3D12Device* mDevice;

    // Buffers
    ID3D12Resource* mVertexBuffer = nullptr;
    ID3D12Resource* mIndexBuffer = nullptr;
    std::vector<ID3D12Resource*> mConstantBuffers;

    ID3D12Resource* mParticleBuffer = nullptr;
    ID3D12Resource* mParticleUploadBuffer = nullptr;

    // Vues
    D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
    D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
    D3D12_VERTEX_BUFFER_VIEW mParticleBufferView = {};

    // Taille maximum du buffer de particules
    UINT mMaxParticles = 10000;
    UINT mParticleStride = 0;
};

// Implementation de la fonction template
template<typename T>
void D3D12Buffers::UpdateConstantBuffer(UINT frameIndex, const T& data) {
    if (frameIndex >= mConstantBuffers.size()) {
        return;
    }

    // Mappage du buffer
    UINT8* pData;
    D3D12_RANGE readRange = { 0, 0 };  // On ne veut pas lire, seulement ecrire
	if (SUCCEEDED(mConstantBuffers[frameIndex]->Map(0, &readRange, reinterpret_cast<void**>(&pData)))) { // On recupere un pointeur vers les donnees
		memcpy(pData, &data, sizeof(T)); // On copie les donnees
		mConstantBuffers[frameIndex]->Unmap(0, nullptr); // On debloque le buffer
    }
}