#pragma once

class Mesh {
public:
    Mesh(ID3D12Device* device);
    ~Mesh();

    // Initialisation avec une geometrie
    void Initialize(const Geometry& geometry);

    // Apparence
    void SetColor(float r, float g, float b, float a = 1.0f);

    // Rendu
    void Draw(ID3D12GraphicsCommandList* cmdList);

    // Accesseurs
    const Geometry& GetGeometry() const { return mGeometry; }
    XMFLOAT3 GetBoundingBox() const;
    D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const { return mVertexBufferView; }
    D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const { return mIndexBufferView; }

private:
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void UpdateVertexBuffer();

    ID3D12Device* mDevice;
    Geometry mGeometry;
    ID3D12Resource* mVertexBuffer = nullptr;
    ID3D12Resource* mIndexBuffer = nullptr;
    D3D12_VERTEX_BUFFER_VIEW mVertexBufferView = {};
    D3D12_INDEX_BUFFER_VIEW mIndexBufferView = {};

    XMMATRIX mWorldMatrix = XMMatrixIdentity();

    bool mVertexBufferDirty = false;
};