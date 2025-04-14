#pragma once


class D3D12RootSignature {
public:
    D3D12RootSignature(ID3D12Device* device);
    ~D3D12RootSignature();

    // Creation d'une signature racine basique
    bool Create();

    // Creation d'une signature racine avec des descripteurs
    bool CreateWithDescriptors();
    bool CreateWithLighting();

    // Accesseurs
    ID3D12RootSignature* GetRootSignature() const { return mRootSignature; }

private:
    ID3D12Device* mDevice;
    ID3D12RootSignature* mRootSignature = nullptr;
    ID3DBlob* mSignatureBlob = nullptr;
    ID3DBlob* mErrorBlob = nullptr;
};