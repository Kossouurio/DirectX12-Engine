#pragma once


//class D3D12Texture {
//public:
//    D3D12Texture(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
//    ~D3D12Texture();
//
//    // Chargement d'une texture depuis un fichier
//    bool LoadFromFile(const std::wstring& filename);
//
//    // Accesseurs
//    ID3D12Resource* GetTextureResource() const { return mTexture; }
//    ID3D12Resource* GetUploadHeap() const { return mTextureUploadHeap; }
//
//    // Utilitaires pour le chargement d'images
//    int LoadImageDataFromFile(BYTE** imageData, D3D12_RESOURCE_DESC& resourceDescription,
//        LPCWSTR filename, int& bytesPerRow);
//    DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);
//    WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
//    int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);
//
//private:
//    ID3D12Device* mDevice;
//    ID3D12GraphicsCommandList* mCommandList;
//
//    ID3D12Resource* mTexture = nullptr;
//    ID3D12Resource* mTextureUploadHeap = nullptr;
//
//};