#include "pch.h"
#include "D3D12Buffers.h"


D3D12Buffers::D3D12Buffers(ID3D12Device* device) : mDevice(device) {
}

D3D12Buffers::~D3D12Buffers() {
    if (mVertexBuffer) mVertexBuffer->Release();
    if (mIndexBuffer) mIndexBuffer->Release();
    for (auto& buffer : mConstantBuffers) {
        if (buffer) buffer->Release();
    }
    if (mParticleBuffer) mParticleBuffer->Release();
    if (mParticleUploadBuffer) mParticleUploadBuffer->Release();
}

void D3D12Buffers::CreateVertexBuffer(const Geometry& geometry) {
    // Taille du buffer de vertex
    const UINT vertexBufferSize = sizeof(geometry.vertices[0]) * geometry.vertices.size();

    //// Description du buffer
    //CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    //CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

    //// Creation du buffer
    //if (FAILED(mDevice->CreateCommittedResource(
    //    &heapProps,
    //    D3D12_HEAP_FLAG_NONE,
    //    &bufferDesc,
    //    D3D12_RESOURCE_STATE_GENERIC_READ,
    //    nullptr,
    //    IID_PPV_ARGS(&mVertexBuffer))))
    //{
    //    throw std::runtime_error("Failed to create vertex buffer.");
    //}

    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    if (FAILED(mDevice->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&mParticleUploadBuffer)))) {
    }


    // Mappage et copie des donnees
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    if (FAILED(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))))
    {
        throw std::runtime_error("Failed to map vertex buffer.");
    }

    memcpy(pVertexDataBegin, geometry.vertices.data(), vertexBufferSize);
    mVertexBuffer->Unmap(0, nullptr);

    // Configuration de la vue du buffer de vertex
    mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
    mVertexBufferView.StrideInBytes = sizeof(geometry.vertices[0]);
    mVertexBufferView.SizeInBytes = vertexBufferSize;
}

void D3D12Buffers::CreateIndexBuffer(const Geometry& geometry) {
    // Taille du buffer d'indices
    const UINT indexBufferSize = sizeof(geometry.indices[0]) * geometry.indices.size();

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
        throw std::runtime_error("Failed to create index buffer.");
    }

    // Mappage et copie des donnees
    UINT8* pIndexDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    if (FAILED(mIndexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin))))
    {
        throw std::runtime_error("Failed to map index buffer.");
    }

    memcpy(pIndexDataBegin, geometry.indices.data(), indexBufferSize);
    mIndexBuffer->Unmap(0, nullptr);

    // Configuration de la vue du buffer d'indices
    mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
    mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;  // Assurez-vous que ce format correspond a votre type d'indice
    mIndexBufferView.SizeInBytes = indexBufferSize;
}

void D3D12Buffers::CreateConstantBuffers(UINT frameCount) {
    // Taille alignee pour les constant buffers (multiple de 256 octets)
    const UINT constantBufferSize = (sizeof(XMMATRIX) + 255) & ~255;

    // Creation de buffers constants pour chaque frame
    mConstantBuffers.resize(frameCount);

    // Description du buffer
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);

    // Creation des buffers pour chaque frame
    for (UINT i = 0; i < frameCount; i++) {
        if (FAILED(mDevice->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&mConstantBuffers[i]))))
        {
            throw std::runtime_error("Failed to create constant buffer.");
        }
    }
}

void D3D12Buffers::CreateParticleBuffer(ID3D12Device* device, UINT maxParticles, UINT particleStride) {
    // Stocker les parametres
    mMaxParticles = maxParticles;
    mParticleStride = particleStride;

    // Calculer la taille du buffer
    const UINT bufferSize = maxParticles * particleStride;

    // Creer le buffer principal sur le GPU
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

    if (FAILED(device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(&mParticleBuffer))))
    {
        throw std::runtime_error("Failed to create particle buffer");
    }

    // Creer le buffer d'upload sur le CPU
    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC uploadBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

    if (FAILED(device->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &uploadBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&mParticleUploadBuffer))))
    {
        throw std::runtime_error("Failed to create particle upload buffer");
    }

    // Initialiser la vue du buffer de particules
    mParticleBufferView.BufferLocation = mParticleBuffer->GetGPUVirtualAddress();
    mParticleBufferView.SizeInBytes = bufferSize;
    mParticleBufferView.StrideInBytes = particleStride;
}

// Implementation sans la valeur par defaut
void D3D12Buffers::UpdateParticleBuffer(const void* data, UINT dataSize, ID3D12GraphicsCommandList* cmdList) {
    if (!cmdList) {
        cmdList = D3D12Window::GetInstance().GetCommandList();/*
        if (!cmdList) {
            HEAD("UpdateParticleBuffer") << "ERREUR : cmdList reste NULL apres tentative de recuperation ! cmdList -> "<< int(cmdList) << ENDL;
            return;
        }*/
    }

    // Copier les donnees dans le buffer d'upload
    UINT8* pDataBegin;
    CD3DX12_RANGE readRange(0, 0);

    if (!data) {
        HEAD("SystemParticle::Update") << "ERREUR: `data` est NULL, tentative d'allocation." << ENDL;
        data = new std::byte[dataSize]; 
    }

    if (SUCCEEDED(mParticleUploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pDataBegin)))) {
        memcpy(pDataBegin, data, dataSize);
        mParticleUploadBuffer->Unmap(0, nullptr);
    }
}


D3D12_VERTEX_BUFFER_VIEW D3D12Buffers::GetParticleBufferView(UINT particleCount) const {
    // Retourner une vue avec la taille actuelle
    D3D12_VERTEX_BUFFER_VIEW view = mParticleBufferView;
    view.SizeInBytes = (std::min)(particleCount * mParticleStride, mMaxParticles * mParticleStride);
    return view;
}