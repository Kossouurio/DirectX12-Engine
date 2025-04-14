#include "pch.h"


LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INPUT:
		Input::ProcessRawInput(lParam);
		return 0;  

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

D3D12Window::D3D12Window()
	: mCbData{},
	mBuffers(nullptr),
	mShaders(nullptr),
	mRootSig(nullptr),
	mPipeline(nullptr)
{
}

D3D12Window::~D3D12Window() {
	CleanUp();
}

void D3D12Window::InitWindow(HINSTANCE hInstance) {
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MsgProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = L"Direct3DWindowClass";
	RegisterClassEx(&wc);

	// Creation de la fenêtre
	mWindow = CreateWindowEx(0, L"Direct3DWindowClass", L"Direct3D 12 cube", WS_OVERLAPPEDWINDOW, 100, 100, mWindowWidth, mWindowHeight, nullptr, nullptr, wc.hInstance, nullptr);

	ShowWindow(mWindow, SW_SHOW);		// Show Window
	UpdateWindow(mWindow);			// Update Window
}

void D3D12Window::InitDevice() {
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	Initialise and Create the Device
	// 
	//	Device : Principal interface between CPU and GPU  
	//	Factory : Allow you to interact with the GPU
	//
	//------------------------------------------------------------------------------------------------------------------------------------------------------------

	// If Debug -> Get Debug Interface for extra debugging and send debug message
	#if defined(DEBUG) || defined(_DEBUG)						
	{
		ID3D12Debug* debugController;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
		debugController->EnableDebugLayer();
	}
	#endif

	CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory));	// -> To Initialize the different feature level (depende on our Windows version) 
	if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&md3dDevice))))
	{// -> If failed : Specifies the display adapter we want the created device to represent
		IDXGIAdapter* pWarpAdapter;
		mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter));
		D3D12CreateDevice(
		pWarpAdapter,
		D3D_FEATURE_LEVEL_12_0,
		IID_PPV_ARGS(&md3dDevice));
	}				
}

void D3D12Window::InitFence() {
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	Create the Fence and Descriptor Sizes
	// 
	//	Fence : Synchronisation that check if GPU's tasks are finished before execute an other
	//			mFence->GetCompletedValue() to check the GPU execution state
	//	Descriptor Sizes : Memory size for a descriptor
	//
	//------------------------------------------------------------------------------------------------------------------------------------------------------------

	// Create a Fence by Device -> CreateFence
	if (FAILED(md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence))))
	{
		throw std::runtime_error("Failed to create fence.");
	}
	mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void D3D12Window::InitMSAA() {
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	Check 4X MSAA Quality Support
	// 
	//	Make a average with 4 pixel to smooth the egdes of 3d Object
	//
	//------------------------------------------------------------------------------------------------------------------------------------------------------------

		// 3. Check 4X MSAA quality level support.
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	md3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels));
	m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
}

void D3D12Window::InitCommand() {
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	Create Command Queue, Command Allocator and Command List
	// 
	//	Way to send commands to the GPU
	// 
	//	Command queue		: une file d'attente that send command from CPU to GPU
	//	Command allocator	: allocate the required memory to store differents command that the GPU will execute
	//	Command List		: GPU command list register by the CPU
	// 
	//  1️) Le CPU enregistre les commandes dans une Command List
	//  2️) Les commandes sont stockees dans un Command Allocator
	//	3️) La Command List est envoyee a une Command Queue
	//	4️) Le GPU execute les commandes dans la Command Queue
	//	5️) Une fois termine, on peut reinitialiser et recommencer
	//
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	 // 4. Create the command queue, command list allocator, and main command list.

	//HEAD("D3D12") << "Debut de InitCommand()" << ENDL;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	if (FAILED(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)))) {
		HEAD("D3D12") << "ERREUR: Impossible de creer la Command Queue !" << ENDL;
		throw std::runtime_error("Failed to create command queue.");
	}

	//HEAD("D3D12") << "Command Queue creee avec succes" << ENDL;

	if (FAILED(md3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mDirectCmdListAlloc)))) {
		HEAD("D3D12") << "ERREUR: Impossible de creer la Command Allocator !" << ENDL;
		throw std::runtime_error("Failed to create command allocator.");
	}

	//HEAD("D3D12") << "Command Allocator creee avec succes" << ENDL;

	if (FAILED(md3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mDirectCmdListAlloc, nullptr, IID_PPV_ARGS(&mCommandList)))) {
		HEAD("D3D12") << "ERREUR: Impossible de creer la Command List !" << ENDL;
		throw std::runtime_error("Failed to create command list.");
	}

	//HEAD("D3D12") << "Command List creee avec succes, adresse: " << char(mCommandList) << ENDL;


	mCommandList->Close();
}


void D3D12Window::InitSwapChain() {
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	Describe and create the Swap Chain
	// 
	//	Manages the display of images on the screen
	//	It's an image queue that store image before show they on the screen
	//   
	//
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	 // 5. Describe and create the swap chain.

	UINT dxgiFactoryFlags = 0;

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = mFrameCount;
    swapChainDesc.Width = mWindowWidth;
    swapChainDesc.Height = mWindowHeight;
    swapChainDesc.Format = mBackBufferFormat;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    if (FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mdxgiFactory))))
    {
        throw std::runtime_error("Failed to create DXGI factory.");
    }

    IDXGISwapChain1* swapChain1;
    if (FAILED(mdxgiFactory->CreateSwapChainForHwnd(mCommandQueue, mWindow, &swapChainDesc, nullptr, nullptr, &swapChain1)))
    {
        throw std::runtime_error("Failed to create swap chain.");
    }

    if (FAILED(swapChain1->QueryInterface(IID_PPV_ARGS(&mSwapChain))))
    {
        throw std::runtime_error("Failed to cast swap chain.");
    }

    mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
}


void D3D12Window::InitDescriptorHeaps() {
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	Create the Descriptor Heaps
	// 
	//	Descriptor : little structure that contain GPU ressource data (texture, buffer, etc.)
	// 
	//	Stock  : 
	//		-> Rendu	: Render Target View (RTV) and Depth Stencil Views (DSV)
	//		-> Shaders	: Shader Ressource View (SRV) and Constant Buffer Views (CBV)
	//		-> UAV (Unordered Access Views) : Access textures/buffer randomly from GPU
	// 
	//
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	 // 6. Create the descriptor heaps the application requires.
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {}; // Describe the heap of RTV (Render Target View)
	rtvHeapDesc.NumDescriptors = mFrameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (FAILED(md3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap))))
	{
		throw std::runtime_error("Failed to create RTV descriptor heap.");
	}
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {}; // Describe the heap of DSV (Depth Stencil View)
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (FAILED(md3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDsvHeap))))
	{
		throw std::runtime_error("Failed to create DSV descriptor heap.");
	}
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {}; // Describe the heap of SRV (Shader Resource View)
	srvHeapDesc.NumDescriptors = 2; // Adjusted to match the number of constant buffers
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	if (FAILED(md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvDescriptorHeap))))
	{
		throw std::runtime_error("Failed to create CBV descriptor heap.");
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	//ID3D12Resource* woodCrateTex = mTextures["woodCrateTex"]->Resource;

	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.Format = woodCrateTex->GetDesc().Format;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MostDetailedMip = 0;
	//srvDesc.Texture2D.MipLevels = woodCrateTex->GetDesc().MipLevels;
	//srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	//md3dDevice->CreateShaderResourceView(woodCrateTex, &srvDesc, hDescriptor);

	//mCommandList->Reset(mDirectCmdListAlloc, nullptr);
}

void D3D12Window::InitRTV() {
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	Create the Render Target View
	// 
	//	Use to draw pixels on the screen
	//	Manage back buffers of the Swap Chain 
	//
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < mFrameCount; i++)
	{
		if (FAILED(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mRenderTargets[i]))))
		{
			throw std::runtime_error("Failed to get swap chain buffer.");
		}
		md3dDevice->CreateRenderTargetView(mRenderTargets[i], nullptr, rtvHandle);
		rtvHandle.Offset(1, mRtvDescriptorSize);
	}
}

void D3D12Window::InitDSV() {
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	Create the Depth/Stencil Buffer and View
	//
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	D3D12_RESOURCE_DESC depthStencilDesc = {};
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Width = mWindowWidth;
	depthStencilDesc.Height = mWindowHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = mDepthStencilFormat;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = mDepthStencilFormat;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	CD3DX12_HEAP_PROPERTIES heapPropertiesDefault(D3D12_HEAP_TYPE_DEFAULT);

	if (FAILED(md3dDevice->CreateCommittedResource(
		&heapPropertiesDefault,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&clearValue,
		IID_PPV_ARGS(&mDepthStencilBuffer))))
	{
		throw std::runtime_error("Failed to create depth/stencil buffer.");
	}
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, mDsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void D3D12Window::InitViewport() {
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	//  Set the Viewport
	// 
	//	The show area of the screen
	//  Manage the position and the size of the screen
	//	Part of the image show on the screen
	//
	//------------------------------------------------------------------------------------------------------------------------------------------------------------
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mWindowWidth);
	mViewport.Height = static_cast<float>(mWindowHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mScissorRect.left = 0;
	mScissorRect.top = 0;
	mScissorRect.right = mWindowWidth;
	mScissorRect.bottom = mWindowHeight;
}



D3D12Window* D3D12Window::Get() {
	static D3D12Window instance; // Si l'instance doit être unique
	return &instance;
}

// Methodes modifiees pour utiliser les classes encapsulees
void D3D12Window::InitConstantBuffer() {
	// Utiliser la classe D3D12Buffers pour creer les constant buffers
	mBuffers->CreateConstantBuffers(mFrameCount);

	// Creer les vues de constant buffer
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	//cbvDesc.SizeInBytes = (sizeof(ConstantBufferData) + 255) & ~255; // Alignement de 256 bytes
	

	for (UINT i = 0; i < mFrameCount; i++) {
		cbvDesc.BufferLocation = mBuffers->GetConstantBuffer(i)->GetGPUVirtualAddress();
		md3dDevice->CreateConstantBufferView(&cbvDesc, mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}
}

// Modification de la methode InitD3D12 pour creer les objets encapsules au bon moment
void D3D12Window::InitD3D12() {
	InitDevice();
	InitFence();
	InitMSAA();
	InitCommand();
	InitSwapChain();
	//LoadTextures(); // Nouveau - Charger la texture
	InitDescriptorHeaps();
	InitRTV();
	InitDSV();
	InitViewport();


	
	mBuffers = std::make_unique<D3D12Buffers>(md3dDevice);
	if (debugMode && logBuffer) {
		if (!mBuffers) {
			HEAD("D3D12") << "echec de l'allocation de D3D12Buffers !" << ENDL;
		}
	}

	mShaders = std::make_unique<D3D12Shader>();
	mRootSig = std::make_unique<D3D12RootSignature>(md3dDevice);
	mRootSig->Create();
	mPipeline = std::make_unique<D3D12Pipeline>(md3dDevice);
	mLights = std::make_unique<D3D12Light>(md3dDevice);

	
	InitConstantBuffer();
	InitLightBuffer();
	InitShader();
	InitRootSignature();

	
	mCommandList->Reset(mDirectCmdListAlloc, nullptr);

	SetupDefaultLights();
	
	InitPipeline();

	
	mCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	
	mFenceValue++;
	mCommandQueue->Signal(mFence, mFenceValue);
	if (mFence->GetCompletedValue() < mFenceValue) {
		mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);
		WaitForSingleObject(mFenceEvent, INFINITE);
		CloseHandle(mFenceEvent);
	}

}

void D3D12Window::InitLightBuffer() {
	mLights->CreateLightBuffer(md3dDevice, mFrameCount);
}

void D3D12Window::SetupDefaultLights() {
	
	if (DefaultLights) {
		mLights->SetAmbientLight({ 0.2f, 0.2f, 0.2f }, 0.01f);
	}else{

		mLights->SetAmbientLight({ 0.5f, 0.5f, 0.5f }, 0.7f);
	}

	//mLights->AddDirectionalLight(
	//	{ -0.5f, -1.0f, 0.5f },   
	//	{ 1.0f, 0.98f, 0.95f },  
	//	0.8f                      
	//);

	
	mLights->AddPointLight(
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },     
		2.0f,                     
		1.5f                      
	);
}

//void D3D12Window::UpdateLights(float dt) {
//	static float totalTime = 0.0f;
//	totalTime += dt;
//	float lightX = 5.0f * cosf(totalTime * 0.5f);
//	float lightZ = 5.0f * sinf(totalTime * 0.5f);
//	mLights->UpdateLightPosition(3, { lightX, 1.0f, lightZ });
//}

 void D3D12Window::UpdateLights(float dt) {
	 XMFLOAT3 playerPos = GameEngine::GetInstance().Getplayer()->GetComponent<ComponentTransform>()->GetPosition();
		mLights->UpdateLightPosition(0, { playerPos.x, playerPos.y - 1.5f, playerPos.z });
	}

ID3D12GraphicsCommandList* D3D12Window::GetCommandList() {
	return mCommandList;
}

HWND D3D12Window::GetHwnd() const {
	return mWindow;
}


void D3D12Window::InitShader() {
	// Utiliser la classe D3D12Shader pour compiler les shaders
	//mShaders->CompileVertexShader(L"../../../src/Engine/color.hlsl", nullptr, "VS", "vs_5_0");
	//mShaders->CompilePixelShader(L"../../../src/Engine/color.hlsl", nullptr, "PS", "ps_5_0");
}

void D3D12Window::InitRootSignature() {
	
	mRootSig->CreateWithDescriptors();
	mRootSig->CreateWithLighting();
}

void D3D12Window::InitPipeline() {
	
	mShaders->CompileVertexShader(L"../../../src/Engine/lighting.hlsl", nullptr, "VS", "vs_5_0");
mShaders->CompilePixelShader(L"../../../src/Engine/lighting.hlsl", nullptr, "PS", "ps_5_0");


	if (!mShaders->GetVertexShaderBlob() || !mShaders->GetPixelShaderBlob()) {
		HEAD("ERROR") << "echec de compilation des shaders d'eclairage" << ENDL;
		return;
	}

	
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 60, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	
	mPipeline->AddInputLayout(inputLayout);

	
	if (!mPipeline->Create(
		mRootSig->GetRootSignature(),
		mShaders->GetVertexShaderBlob(),
		mShaders->GetPixelShaderBlob(),
		mBackBufferFormat,
		true,
		mDepthStencilFormat
	))
	{
	}
}

void D3D12Window::InitParticlePipeline() {
	
	mShaders->CompileVertexShader(L"../../../src/Engine/particle.hlsl", nullptr, "VS", "vs_5_0");
	mShaders->CompilePixelShader(L"../../../src/Engine/particle.hlsl", nullptr, "PS", "ps_5_0");

	mParticlePipeline = std::make_unique<D3D12Pipeline>(md3dDevice);


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	mParticlePipeline->AddInputLayout(inputLayout);

	
	if (!mParticlePipeline->Create(
		mRootSig->GetRootSignature(),
		mShaders->GetVertexShaderBlob(),
		mShaders->GetPixelShaderBlob(),
		mBackBufferFormat,
		true,  
		mDepthStencilFormat,
		true
	)) {
		HEAD("ERROR") << "echec de creation du pipeline pour les particules" << ENDL;
	}

}

void D3D12Window::InitTexture(const std::wstring& textureFilename) {

	//if (mTextures->LoadFromFile(textureFilename)) {
	//	
	//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	srvDesc.Format = mTextures->GetTextureResource()->GetDesc().Format;
	//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//	srvDesc.Texture2D.MipLevels = 1;

	//	
	//	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(mCbvHeap->GetCPUDescriptorHandleForHeapStart());
	//	srvHandle.Offset(1, mCbvSrvDescriptorSize);

	//	md3dDevice->CreateShaderResourceView(
	//		mTextures->GetTextureResource(),
	//		&srvDesc,
	//		srvHandle
	//	);
	//}
}


void D3D12Window::Draw(float dt) {
	
	UpdateCamera();

	
	XMMATRIX viewMatrix = cameraComponent->GetCamera()->GetViewMatrix();
	XMMATRIX projMatrix = cameraComponent->GetCamera()->GetProjectionMatrix();

	
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, cameraComponent->GetCamera()->GetPositionMatrix());

	
	UpdateLights(dt);

	
	mLights->UpdateLightBuffer(mFrameIndex, camPos);

	
	struct ParticleConstantBuffer {
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPosition;
		float deltaTime;
	};

	ParticleConstantBuffer particleCB;
	particleCB.viewProjection = XMMatrixTranspose(viewMatrix * projMatrix);
	particleCB.cameraPosition = camPos;
	particleCB.deltaTime = dt;

	
	mDirectCmdListAlloc->Reset();
	HRESULT hr = mCommandList->Reset(mDirectCmdListAlloc, mPipeline->GetPipelineState());
	if (FAILED(hr)) {
		HEAD("D3D12") << "ERREUR : Reset() de mCommandList a echoue !" << ENDL;
	}

	
	CD3DX12_RESOURCE_BARRIER barriers[] = {
		CD3DX12_RESOURCE_BARRIER::Transition(
			mRenderTargets[mFrameIndex],
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		),
		CD3DX12_RESOURCE_BARRIER::Transition(
			mDepthStencilBuffer,
			D3D12_RESOURCE_STATE_COMMON, 
			D3D12_RESOURCE_STATE_DEPTH_WRITE
		)
	};
	mCommandList->ResourceBarrier(2, barriers);

	
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mFrameIndex,
		mRtvDescriptorSize
	);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(mDsvHeap->GetCPUDescriptorHandleForHeapStart());
	mCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

	
	const float clearColor[] = { 0.008f, 0.0f, 0.06f, 1.0f }; 
	mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	mCommandList->ClearDepthStencilView(
		mDsvHeap->GetCPUDescriptorHandleForHeapStart(),
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr
	);

	
	mCommandList->SetGraphicsRootSignature(mRootSig->GetRootSignature());

	// Lier le constant buffer de lumieres
	mCommandList->SetGraphicsRootConstantBufferView(
		1,
		mLights->GetLightBuffer(mFrameIndex)->GetGPUVirtualAddress()
	);

	// Configuration des descriptor heaps
	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvDescriptorHeap };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// Lier la texture
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart()); // Recuperer le premier handle dans le heap de descripteurs CBV/SRV/UAV 
	srvHandle.ptr += 0 * mCbvSrvDescriptorSize; // Decaler pour la texture
	mCommandList->SetGraphicsRootDescriptorTable(2, srvHandle); // Lier la texture au shader

	//// Lier la texture
	//if (texture) {
	//	auto srvHandle = GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(); // Recuperer le premier handle
	//	srvHandle.ptr += 1 * renderer->GetCbvSrvDescriptorSize(); // Decaler pour la texture
	//	renderer->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvHandle); // Lier la texture au shader 
	//}


	
	mCommandList->RSSetViewports(1, &mViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	SystemManager* systemManager = &GameEngine::GetInstance().GetSystemManager();
	SystemRender* renderSystem = systemManager->GetSystem<SystemRender>();

	if (renderSystem) {
		renderSystem->Update(dt, GameEngine::GetInstance().GetEntityManager().GetAllEntities());
	}
	else {
		HEAD("ERROR") << "RenderSystem introuvable dans SystemManager !" << ENDL;
	}

	
	CD3DX12_RESOURCE_BARRIER finalBarriers[] = {
		CD3DX12_RESOURCE_BARRIER::Transition(
			mRenderTargets[mFrameIndex],
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		),
		CD3DX12_RESOURCE_BARRIER::Transition(
			mDepthStencilBuffer,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			D3D12_RESOURCE_STATE_COMMON 
		)
	};
	mCommandList->ResourceBarrier(2, finalBarriers);

	
	if (FAILED(mCommandList->Close())) {
		throw std::runtime_error("Failed to close command list.");
	}

	
	ID3D12CommandList* ppCommandLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	
	const UINT64 currentFenceValue = mFenceValue;
	if (FAILED(mCommandQueue->Signal(mFence, currentFenceValue))) {
		throw std::runtime_error("Failed to signal command queue.");
	}
	mFenceValue++;

	// Presentation de l'image sur l'ecran (swap chain)
	if (FAILED(mSwapChain->Present(0, 0))) { // 0, 0 = pas de synchro verticale ni flags de presentation speciaux (comme le mode plein ecran)
		throw std::runtime_error("Failed to present swap chain."); 
	}

	
	if (mFence->GetCompletedValue() < currentFenceValue) {
		if (FAILED(mFence->SetEventOnCompletion(currentFenceValue, mFenceEvent))) {
			throw std::runtime_error("Failed to set fence event.");
		}
		WaitForSingleObject(mFenceEvent, INFINITE);
	}

	
	mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
}


void D3D12Window::CleanUp() {
	
	if (mFence && mCommandQueue) {
		mCommandQueue->Signal(mFence, mFenceValue);
		if (mFence->GetCompletedValue() < mFenceValue) {
			mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);
			WaitForSingleObject(mFenceEvent, INFINITE);
			CloseHandle(mFenceEvent);
		}
	}
	for (auto mesh : mMeshes) {
		delete mesh;
	}

	mMeshes.clear();

	
	mPipeline.reset();
	mRootSig.reset();
	mShaders.reset();
	mLights.reset();  
	mBuffers.reset();
	

	
	for (int i = 0; i < mFrameCount; i++) {
		if (mRenderTargets[i]) mRenderTargets[i]->Release();
	}

	if (mDepthStencilBuffer) mDepthStencilBuffer->Release();
	if (mSrvDescriptorHeap) mSrvDescriptorHeap->Release();
	if (mDsvHeap) mDsvHeap->Release();
	if (mRtvHeap) mRtvHeap->Release();
	if (mSwapChain) mSwapChain->Release();
	if (mCommandList) mCommandList->Release();
	if (mDirectCmdListAlloc) mDirectCmdListAlloc->Release();
	if (mCommandQueue) mCommandQueue->Release();
	if (mFence) mFence->Release();
	if (md3dDevice) md3dDevice->Release();
	if (mdxgiFactory) mdxgiFactory->Release();
}


void D3D12Window::SetCameraComponent(ComponentCamera* camera) {
	cameraComponent = camera;
}

void D3D12Window::UpdateCamera() {
	if (!cameraComponent) {
		HEAD("ERROR") << "Aucune camera attachee a D3D12Window !" << ENDL;
		return;
	}
	else {
		cameraComponent->GetCamera()->SetProjectionMatrix(
			cameraComponent->GetCamera()->GetFovY(),
			static_cast<float>(mWindowWidth) / static_cast<float>(mWindowHeight),
			0.1f,
			100.0f
		);
		cameraComponent->SyncWithTransform();
	}
}

