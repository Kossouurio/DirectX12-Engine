#pragma once

// Types de lumieres supportes
enum class LightType {
    Directional,
    Point,
    Spot
};

// Structure de donnees pour une lumiere
struct Light {
    DirectX::XMFLOAT4 position;     // Pour point et spot (w=1.0), direction pour directional (w=0.0)
    DirectX::XMFLOAT4 direction;    // Pour spot et directional, ignore pour point
    DirectX::XMFLOAT4 color;        // rgb = couleur, a = intensite
    DirectX::XMFLOAT4 params;       // x = rayon, y = falloff, z = spotAngle (en radians), w = type

    Light() {
        position = { 0.0f, 0.0f, 0.0f, 1.0f };
        direction = { 0.0f, -1.0f, 0.0f, 0.0f };
        color = { 1.0f, 1.0f, 1.0f, 1.0f };
        params = { 10.0f, 1.0f, DirectX::XM_PIDIV4, 0.0f }; // Point light par defaut
    }
};

// Constantes pour le buffer de lumieres
struct LightConstants {
    DirectX::XMFLOAT4 ambientLight;    // rgb = couleur ambiante, a = intensite
    DirectX::XMFLOAT4 eyePosition;     // Position de la camera pour les calculs de speculaire
    Light lights[8];                   // Tableau de lumieres
    UINT lightCount;                   // Nombre de lumieres actives
    DirectX::XMFLOAT3 padding;         // Pour aligner a 16 bytes
};

class D3D12Light {
public:
    D3D12Light(ID3D12Device* device);
    ~D3D12Light();

    // Initialisation du buffer constant pour les lumieres
    void CreateLightBuffer(ID3D12Device* device, UINT frameCount);

    // Ajout et manipulation des lumieres
    void SetAmbientLight(const DirectX::XMFLOAT3& color, float intensity);
    int AddDirectionalLight(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& color, float intensity);
    int AddPointLight(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& color, float intensity, float radius);
    int AddSpotLight(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction,
        const DirectX::XMFLOAT3& color, float intensity, float radius, float spotAngle);

    // Mise a jour des parametres de lumiere
    void UpdateLightPosition(int lightIndex, const DirectX::XMFLOAT3& position);
    void UpdateLightDirection(int lightIndex, const DirectX::XMFLOAT3& direction);
    void UpdateLightColor(int lightIndex, const DirectX::XMFLOAT3& color, float intensity);
    void UpdateLightParams(int lightIndex, float radius, float falloff, float spotAngle = DirectX::XM_PIDIV4);
    void RemoveLight(int lightIndex);
    void ClearAllLights();

    // Mise a jour du buffer constant
    void UpdateLightBuffer(UINT frameIndex, const DirectX::XMFLOAT3& eyePosition);

    // Accesseurs
    ID3D12Resource* GetLightBuffer(UINT frameIndex) const { return mLightBuffers[frameIndex]; }
    const LightConstants& GetLightData() const { return mLightData; }

private:
    ID3D12Device* mDevice;
    std::vector<ID3D12Resource*> mLightBuffers;     // Un buffer par frame dans le swapchain
    LightConstants mLightData;                      // Donnees de lumiere
    std::vector<bool> mActiveLights;               // Pour suivre les slots de lumiere utilises
};