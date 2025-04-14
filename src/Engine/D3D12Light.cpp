#include "pch.h"
#include "D3D12Light.h"
#include "D3D12Window.h"

D3D12Light::D3D12Light(ID3D12Device* device) : mDevice(device) {
    // Initialisation des valeurs par defaut
    mLightData.ambientLight = { 0.2f, 0.2f, 0.2f, 1.0f };
    mLightData.eyePosition = { 0.0f, 0.0f, 0.0f, 1.0f };
    mLightData.lightCount = 0;
    mLightData.padding = { 0.0f, 0.0f, 0.0f };

    mActiveLights.resize(8, false); // Maximum de 8 lumieres supportees
}

D3D12Light::~D3D12Light() {
    // Liberation des ressources
    for (auto& buffer : mLightBuffers) {
        if (buffer) buffer->Release();
    }
}

void D3D12Light::CreateLightBuffer(ID3D12Device* device, UINT frameCount) {
    // Taille alignee pour les constant buffers (multiple de 256 octets)
    const UINT constantBufferSize = (sizeof(LightConstants) + 255) & ~255;

    // Creation de buffers constants pour chaque frame
    mLightBuffers.resize(frameCount);

    // Description du buffer
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);

    // Creation des buffers pour chaque frame
    for (UINT i = 0; i < frameCount; i++) {
        if (FAILED(device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&mLightBuffers[i]))))
        {
            throw std::runtime_error("Failed to create light constant buffer.");
        }
    }
}

void D3D12Light::SetAmbientLight(const DirectX::XMFLOAT3& color, float intensity) {
    mLightData.ambientLight.x = color.x;
    mLightData.ambientLight.y = color.y;
    mLightData.ambientLight.z = color.z;
    mLightData.ambientLight.w = intensity;
}

int D3D12Light::AddDirectionalLight(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& color, float intensity) {
    // Trouver un slot libre
    int index = -1;
    for (int i = 0; i < mActiveLights.size(); i++) {
        if (!mActiveLights[i]) {
            index = i;
            break;
        }
    }

    if (index == -1 || index >= 8) {
        // Trop de lumieres, impossible d'en ajouter une autre
        return -1;
    }

    // Normaliser la direction
    XMVECTOR dirVec = XMLoadFloat3(&direction);
    dirVec = XMVector3Normalize(dirVec);
    XMFLOAT3 normalizedDir;
    XMStoreFloat3(&normalizedDir, dirVec);

    // Initialiser la lumiere directionnelle
    Light& light = mLightData.lights[index];
    light.position = { 0.0f, 0.0f, 0.0f, 0.0f }; // w = 0 indique une lumiere directionnelle
    light.direction = { normalizedDir.x, normalizedDir.y, normalizedDir.z, 0.0f };
    light.color = { color.x, color.y, color.z, intensity };
    light.params = { 0.0f, 0.0f, 0.0f, static_cast<float>(LightType::Directional) };

    mActiveLights[index] = true;
    if (static_cast<UINT>(index + 1) > mLightData.lightCount) {
        mLightData.lightCount = static_cast<UINT>(index + 1);
    }

    return index;
}

int D3D12Light::AddPointLight(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& color, float intensity, float radius) {
    int index = -1;
    for (int i = 0; i < mActiveLights.size(); i++) {
        if (!mActiveLights[i]) {
            index = i;
            break;
        }
    }

    if (index == -1 || index >= 8) {
        return -1;
    }

    Light& light = mLightData.lights[index];
    light.position = { position.x, position.y, position.z, 1.0f }; // w = 1 pour point/spot
    light.direction = { 0.0f, 0.0f, 0.0f, 0.0f }; // Pas utilise pour les lumieres ponctuelles
    light.color = { color.x, color.y, color.z, intensity };
    light.params = { radius, 1.0f, 0.0f, static_cast<float>(LightType::Point) };

    mActiveLights[index] = true;
    if (static_cast<UINT>(index + 1) > mLightData.lightCount) {
        mLightData.lightCount = static_cast<UINT>(index + 1);
    }

    return index;
}

int D3D12Light::AddSpotLight(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction,
    const DirectX::XMFLOAT3& color, float intensity, float radius, float spotAngle) {
    int index = -1;
    for (int i = 0; i < mActiveLights.size(); i++) {
        if (!mActiveLights[i]) {
            index = i;
            break;
        }
    }

    if (index == -1 || index >= 8) {
        return -1;
    }

    // Normaliser la direction
    XMVECTOR dirVec = XMLoadFloat3(&direction);
    dirVec = XMVector3Normalize(dirVec);
    XMFLOAT3 normalizedDir;
    XMStoreFloat3(&normalizedDir, dirVec);

    Light& light = mLightData.lights[index];
    light.position = { position.x, position.y, position.z, 1.0f };
    light.direction = { normalizedDir.x, normalizedDir.y, normalizedDir.z, 0.0f };
    light.color = { color.x, color.y, color.z, intensity };
    light.params = { radius, 1.0f, spotAngle, static_cast<float>(LightType::Spot) };

    mActiveLights[index] = true;
    if (static_cast<UINT>(index + 1) > mLightData.lightCount) {
        mLightData.lightCount = static_cast<UINT>(index + 1);
    }

    return index;
}

void D3D12Light::UpdateLightPosition(int lightIndex, const DirectX::XMFLOAT3& position) {
    if (lightIndex < 0 || lightIndex >= mActiveLights.size() || !mActiveLights[lightIndex]) {
        return;
    }

    mLightData.lights[lightIndex].position = { position.x, position.y, position.z,
        mLightData.lights[lightIndex].position.w };
}

void D3D12Light::UpdateLightDirection(int lightIndex, const DirectX::XMFLOAT3& direction) {
    if (lightIndex < 0 || lightIndex >= mActiveLights.size() || !mActiveLights[lightIndex]) {
        return;
    }

    // Normaliser la direction
    XMVECTOR dirVec = XMLoadFloat3(&direction);
    dirVec = XMVector3Normalize(dirVec);
    XMFLOAT3 normalizedDir;
    XMStoreFloat3(&normalizedDir, dirVec);

    mLightData.lights[lightIndex].direction = { normalizedDir.x, normalizedDir.y, normalizedDir.z, 0.0f };
}

void D3D12Light::UpdateLightColor(int lightIndex, const DirectX::XMFLOAT3& color, float intensity) {
    if (lightIndex < 0 || lightIndex >= mActiveLights.size() || !mActiveLights[lightIndex]) {
        return;
    }

    mLightData.lights[lightIndex].color = { color.x, color.y, color.z, intensity };
}

void D3D12Light::UpdateLightParams(int lightIndex, float radius, float falloff, float spotAngle) {
    if (lightIndex < 0 || lightIndex >= mActiveLights.size() || !mActiveLights[lightIndex]) {
        return;
    }

    Light& light = mLightData.lights[lightIndex];
    light.params.x = radius;
    light.params.y = falloff;
    light.params.z = spotAngle;
}

void D3D12Light::RemoveLight(int lightIndex) {
    if (lightIndex < 0 || lightIndex >= mActiveLights.size()) {
        return;
    }

    mActiveLights[lightIndex] = false;

    // Recalculer le nombre de lumieres actives
    mLightData.lightCount = 0;
    for (int i = mActiveLights.size() - 1; i >= 0; i--) {
        if (mActiveLights[i]) {
            mLightData.lightCount = i + 1;
            break;
        }
    }
}

void D3D12Light::ClearAllLights() {
    for (int i = 0; i < mActiveLights.size(); i++) {
        mActiveLights[i] = false;
    }
    mLightData.lightCount = 0;
}

void D3D12Light::UpdateLightBuffer(UINT frameIndex, const DirectX::XMFLOAT3& eyePosition) {
    if (frameIndex >= mLightBuffers.size()) {
        return;
    }

    // Mettre a jour la position de la camera
    mLightData.eyePosition = { eyePosition.x, eyePosition.y, eyePosition.z, 1.0f };

    // Mappage du buffer
    UINT8* pData;
    D3D12_RANGE readRange = { 0, 0 };  // On ne veut pas lire, seulement ecrire
    if (SUCCEEDED(mLightBuffers[frameIndex]->Map(0, &readRange, reinterpret_cast<void**>(&pData)))) {
        memcpy(pData, &mLightData, sizeof(mLightData));
        mLightBuffers[frameIndex]->Unmap(0, nullptr);
    }
}