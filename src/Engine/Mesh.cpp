#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ID3D12Device* device) : mDevice(device) {
}

Mesh::~Mesh() {
    if (mVertexBuffer) mVertexBuffer->Release();
    if (mIndexBuffer) mIndexBuffer->Release();
}

void Mesh::Initialize(const Geometry& geometry) {
    mGeometry = geometry;
    CreateVertexBuffer();
    CreateIndexBuffer();
}

void Mesh::SetColor(float r, float g, float b, float a) {
    for (auto& vertex : mGeometry.vertices) {
        vertex.color = { r, g, b, a };
    }

    mVertexBufferDirty = true;

    // Si le buffer existe deja, mettre a jour les donnees
    if (mVertexBuffer) {
        UpdateVertexBuffer();
    }
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList) {
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->IASetVertexBuffers(0, 1, &mVertexBufferView);
    cmdList->IASetIndexBuffer(&mIndexBufferView);
    cmdList->DrawIndexedInstanced((UINT)mGeometry.indices.size(), 1, 0, 0, 0);
}

XMFLOAT3 Mesh::GetBoundingBox() const {
    if (mGeometry.vertices.empty()) return XMFLOAT3(1.0f, 1.0f, 1.0f); // Valeur par defaut

    XMFLOAT3 minBounds = { FLT_MAX, FLT_MAX, FLT_MAX };
    XMFLOAT3 maxBounds = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

    for (const auto& vertex : mGeometry.vertices) {
        minBounds.x = fminf(minBounds.x, vertex.pos.x);
        minBounds.y = fminf(minBounds.y, vertex.pos.y);
        minBounds.z = fminf(minBounds.z, vertex.pos.z);

        maxBounds.x = fmaxf(maxBounds.x, vertex.pos.x);
        maxBounds.y = fmaxf(maxBounds.y, vertex.pos.y);
        maxBounds.z = fmaxf(maxBounds.z, vertex.pos.z);
    }

    return XMFLOAT3(
        maxBounds.x - minBounds.x,
        maxBounds.y - minBounds.y,
        maxBounds.z - minBounds.z
    );
}




void Mesh::CreateVertexBuffer() {
    // Taille du buffer de vertex
    const UINT vertexBufferSize = sizeof(mGeometry.vertices[0]) * mGeometry.vertices.size();

    // Description du buffer
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

    // Creation du buffer
    if (FAILED(mDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&mVertexBuffer))))
    {
        throw std::runtime_error("Failed to create vertex buffer for mesh.");
    }

    // Mappage et copie des donnees
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    if (FAILED(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))))
    {
        throw std::runtime_error("Failed to map vertex buffer for mesh.");
    }

    memcpy(pVertexDataBegin, mGeometry.vertices.data(), vertexBufferSize);
    mVertexBuffer->Unmap(0, nullptr);

    // Configuration de la vue du buffer de vertex
    mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
    mVertexBufferView.StrideInBytes = sizeof(mGeometry.vertices[0]);
    mVertexBufferView.SizeInBytes = vertexBufferSize;

    mVertexBufferDirty = false;
}

void Mesh::CreateIndexBuffer() {
    // Taille du buffer d'indices
    const UINT indexBufferSize = sizeof(mGeometry.indices[0]) * mGeometry.indices.size();

    // Description du buffer
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

    // Creation du buffer
    if (FAILED(mDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&mIndexBuffer))))
    {
        throw std::runtime_error("Failed to create index buffer for mesh.");
    }

    // Mappage et copie des donnees
    UINT8* pIndexDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    if (FAILED(mIndexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin))))
    {
        throw std::runtime_error("Failed to map index buffer for mesh.");
    }

    memcpy(pIndexDataBegin, mGeometry.indices.data(), indexBufferSize);
    mIndexBuffer->Unmap(0, nullptr);

    // Configuration de la vue du buffer d'indices
    mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
    mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;  // Utilise R16_UINT si tu utilises uint16_t pour les indices
    mIndexBufferView.SizeInBytes = indexBufferSize;
}

void Mesh::UpdateVertexBuffer() {
    if (!mVertexBufferDirty || !mVertexBuffer) {
        return;
    }

    // Taille du buffer de vertex
    const UINT vertexBufferSize = sizeof(mGeometry.vertices[0]) * mGeometry.vertices.size();

    // Mappage et copie des donnees
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    if (FAILED(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))))
    {
        throw std::runtime_error("Failed to map vertex buffer for update.");
    }

    memcpy(pVertexDataBegin, mGeometry.vertices.data(), vertexBufferSize);
    mVertexBuffer->Unmap(0, nullptr);

    mVertexBufferDirty = false;
}