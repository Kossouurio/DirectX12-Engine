#pragma once
class D3D12Buffers;
class D3D12Shader;
class D3D12RootSignature;
class D3D12Pipeline;
class D3D12Light;
class SimpleParticleSystem;
class Mesh;

struct ConstantBufferData {
    XMMATRIX World;
    XMMATRIX View;
    XMMATRIX Projection;
    XMMATRIX WorldViewProj;
    XMFLOAT4 Time; // x = temps total, y = delta-temps, z,w = inutilises
};

class D3D12Window {
private:
    // ==============================
    //  Variables de la Fen�tre
    // ==============================
    HWND mWindow = nullptr;
   

    // ==============================
    //  Interface GPU
    // ==============================
    ID3D12Device* md3dDevice = nullptr;
    IDXGIFactory4* mdxgiFactory = nullptr;
    ID3D12CommandQueue* mCommandQueue = nullptr;
    ID3D12CommandAllocator* mDirectCmdListAlloc = nullptr;
    ID3D12GraphicsCommandList* mCommandList = nullptr;
    IDXGISwapChain3* mSwapChain = nullptr;

    // ==============================
    //  Synchronisation
    // ==============================
    ID3D12Fence* mFence = nullptr;
    UINT64 mFenceValue = 0;
    HANDLE mFenceEvent = nullptr;

    // ==============================
    //  Buffers et Heaps
    // ==============================
    ID3D12DescriptorHeap* mRtvHeap = nullptr; // Render Target View
    ID3D12DescriptorHeap* mDsvHeap = nullptr; // Depth Stencil View
    ID3D12DescriptorHeap* mSrvDescriptorHeap = nullptr; // Constant Buffer View
    ID3D12DescriptorHeap* mParticleHeap = nullptr;
    ID3D12Resource* mRenderTargets[2] = { nullptr, nullptr };
    ID3D12Resource* mDepthStencilBuffer = nullptr;
    // ==============================
    //  Formats et Parametres DX12
    // ==============================
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT mFrameIndex = 0; // Index de l'image courante dans le swap chain (0 ou 1)
	const UINT mFrameCount = 2; // 2 buffers en back buffer pour le swap chain (double buffering)
    UINT mRtvDescriptorSize = 0;
    UINT mDsvDescriptorSize = 0;
    UINT mCbvSrvDescriptorSize = 0;
    UINT m4xMsaaQuality = 0;
    bool m4xMsaaState = false;

    
    // ==============================
    //  Geometrie et Matrices
    // ==============================
    D3D12_VIEWPORT mViewport;
    D3D12_RECT mScissorRect;

    ConstantBufferData mCbData;

    XMMATRIX mWorldMatrix = XMMatrixIdentity();
    XMMATRIX mViewMatrix = XMMatrixLookAtLH(
        XMVectorSet(3.0f, 3.0f, -5.0f, 1.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );

    XMMATRIX mProjectionMatrix = XMMatrixPerspectiveFovLH(
        XM_PIDIV4,
        static_cast<float>(mWindowWidth) / static_cast<float>(mWindowHeight),
        0.1f,
        100.0f
    );

    // ==============================
    //  Camera
    // ==============================
    ComponentCamera* cameraComponent;

    struct SimpleParticle {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 velocity;
        DirectX::XMFLOAT4 color;
        float size;
        float life;
    };

    // Ressources pour le test direct des particules
    ID3D12Resource* mSimpleParticleBuffer = nullptr;
    ID3D12Resource* mSimpleParticleUploadBuffer = nullptr;
    D3D12_VERTEX_BUFFER_VIEW mSimpleParticleView = {};
    std::vector<SimpleParticle> mSimpleParticles;
    ID3D12PipelineState* mSimpleParticlePSO = nullptr;

    // ==============================
    //  Classes encapsulees
    // ==============================
    unique_ptr<D3D12Buffers> mBuffers;
    unique_ptr<D3D12Shader> mShaders;
    unique_ptr<D3D12RootSignature> mRootSig;
    unique_ptr<D3D12Pipeline> mPipeline;
	unique_ptr<D3D12Light> mLights;
    std::vector<Mesh*> mMeshes;
    unique_ptr<D3D12Pipeline> mParticlePipeline;
    // ==============================
    //  Fonctions internes
    // ==============================
    void InitDevice();
    void InitFence();
    void InitMSAA();
    void InitCommand();
    void InitSwapChain();
    void InitDescriptorHeaps();
    void InitRTV();
    void InitDSV();
    void InitViewport();

    // Ces methodes utilisent maintenant les classes encapsulees
    void InitConstantBuffer();
    void InitLightBuffer();
    void InitShader();
    void InitRootSignature();
    void InitPipeline();
    void InitParticlePipeline();
    void InitTexture(const wstring& textureFilename);
    void SetupDefaultLights();

    std::unordered_map<std::string, Texture*> mTextures;
    void LoadTextures(); // Nouveau - Chargement des textures

public:
    // ==============================
    //  Fonctions publiques
    // ==============================
    const int mWindowWidth = 1080;
    const int mWindowHeight = 740;

    D3D12Window();
    ~D3D12Window();

    void InitWindow(HINSTANCE hInstance);
    void InitD3D12();
    void Draw(float dt);
    void CleanUp();
	void SetCameraComponent(ComponentCamera* cameraComponent);
    void UpdateCamera();
	void UpdateLights(float dt);
    ID3D12Device* GetDevice() const { return md3dDevice; }
    D3D12Buffers* GetBuffers() { return mBuffers.get(); }
    ID3D12GraphicsCommandList* GetCommandList();
	bool DefaultLights = true;
	ConstantBufferData& GetCbData() { return mCbData; }
	ID3D12PipelineState* GetPipeline() { return mPipeline->GetPipelineState(); }

    HWND GetHwnd() const;

    static D3D12Window* Get();
    static D3D12Window& GetInstance() {
        static D3D12Window instance;   
        return instance;
    }
       
	DXGI_FORMAT GetBackBufferFormat() const { return mBackBufferFormat; }
	DXGI_FORMAT GetDepthStencilFormat() const { return mDepthStencilFormat; }
    D3D12Shader* GetShaders() { return mShaders.get(); }
    D3D12RootSignature* GetRootSignature() { return mRootSig.get(); }
	ID3D12RootSignature* GetRootSig() { return mRootSig->GetRootSignature(); }
	UINT GetFrameIndex() const { return mFrameIndex; }
    ID3D12PipelineState* GetParticlePipeline() { return mParticlePipeline ? mParticlePipeline->GetPipelineState() : nullptr; }
	int GetWindowWidth() const { return mWindowWidth; }
	int GetWindowHeight() const { return mWindowHeight; }

	std::unordered_map<std::string, Texture*>& GetTextures() { return mTextures; }
   
};
