#pragma once


class D3D12Pipeline {
public:
    D3D12Pipeline(ID3D12Device* device);
    ~D3D12Pipeline();

    // Creation du pipeline state object
    bool Create(
        ID3D12RootSignature* rootSignature,
        ID3DBlob* vertexShader,
        ID3DBlob* pixelShader,
        DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
        bool depthEnabled = false,
        DXGI_FORMAT dsvFormat = DXGI_FORMAT_D24_UNORM_S8_UINT,
        bool enableBlending = false
    );

    // Ajout des elements d'entree pour le layout du vertex
    void AddInputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElements);

    // Accesseurs
    ID3D12PipelineState* GetPipelineState() const { return mPipelineState; }

private:
    ID3D12Device* mDevice;
    ID3D12PipelineState* mPipelineState = nullptr;
    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
};