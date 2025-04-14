#include "pch.h"
#include "D3D12Window.h"

LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Compile un shader HLSL en bytecode
ID3DBlob* D3D12Window::CompileShader(
	const std::wstring& filename,
	const D3D_SHADER_MACRO* defines,
	const std::string& entrypoint,
	const std::string& target)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr;

	ID3DBlob* byteCode = nullptr;
	ID3DBlob* errors = nullptr;
	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (FAILED(hr) && errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

	return byteCode;
}

D3D12Window::D3D12Window() 
    : mCbData{},
      mCbvHeap(nullptr),
      mDsvHeap(nullptr),
      mIndexBufferView{},
      mRtvHeap(nullptr),
      mScissorRect{},
      mVertexBufferView{},
      mViewport{} 
{
}

D3D12Window::~D3D12Window() {
	//CleanUp();
}

void D3D12Window::InitWindow(HINSTANCE hInstance) {
	// Generate

	GeometryGenerator::GenerateSphere(mGeometry,1,20);
	//GeometryGenerator::GenerateCube(mGeometry2);


	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MsgProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = L"Direct3DWindowClass";
	RegisterClassEx(&wc);

	// Creation de la fenêtre
	mWindow = CreateWindowEx(0, L"Direct3DWindowClass", L"Direct3D 12 cube", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, nullptr, nullptr, wc.hInstance, nullptr);

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
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	if (FAILED(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue))))
	{
		throw std::runtime_error("Failed to create command queue.");
	}
	if (FAILED(md3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mDirectCmdListAlloc))))
	{
		throw std::runtime_error("Failed to create command allocator.");
	}
	if (FAILED(md3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mDirectCmdListAlloc, nullptr, IID_PPV_ARGS(&mCommandList))))
	{
		throw std::runtime_error("Failed to create command list.");
	}
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
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = mFrameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (FAILED(md3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap))))
	{
		throw std::runtime_error("Failed to create RTV descriptor heap.");
	}
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (FAILED(md3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDsvHeap))))
	{
		throw std::runtime_error("Failed to create DSV descriptor heap.");
	}
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
	cbvHeapDesc.NumDescriptors = 2; // Adjusted to match the number of constant buffers
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	if (FAILED(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap))))
	{
		throw std::runtime_error("Failed to create CBV descriptor heap.");
	}
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
	mViewport.Width = static_cast<float>(800);
	mViewport.Height = static_cast<float>(600);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mScissorRect.left = 0;
	mScissorRect.top = 0;
	mScissorRect.right = 800;
	mScissorRect.bottom = 600;
}
// 10 
void D3D12Window::InitConstantBuffer() {
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.SizeInBytes = (sizeof(ConstantBufferMatrix) + 255) & ~255; // Alignement de 256 bytes

	D3D12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(cbvDesc.SizeInBytes); // Utiliser la taille alignee

	for (int i = 0; i < 2; ++i)
	{
		if (FAILED(md3dDevice->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mConstantBuffers[i]))))
		{
			throw std::runtime_error("Failed to create constant buffer.");
		}

		cbvDesc.BufferLocation = mConstantBuffers[i]->GetGPUVirtualAddress();
		md3dDevice->CreateConstantBufferView(&cbvDesc, mCbvHeap->GetCPUDescriptorHandleForHeapStart());
	}
}



void D3D12Window::InitVertexBuffer() {
	const UINT vertexBufferSize = sizeof(mGeometry.vertices[0]) * mGeometry.vertices.size();
	CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	if (FAILED(md3dDevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mVertexBuffer))))
	{
		throw std::runtime_error("Failed to create vertex buffer.");
	}
	UINT8* pVertexDataBegin;
	CD3DX12_RANGE readRange(0, 0);
	if (FAILED(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))))
	{
		throw std::runtime_error("Failed to map vertex buffer.");
	}
	memcpy(pVertexDataBegin, mGeometry.vertices.data(), vertexBufferSize);
	mVertexBuffer->Unmap(0, nullptr);
	mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
	mVertexBufferView.StrideInBytes = sizeof(mGeometry.vertices[0]);
	mVertexBufferView.SizeInBytes = vertexBufferSize;

	//const UINT vertexBufferSize = sizeof(vertices);
	//D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	//CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	//if (FAILED(md3dDevice->CreateCommittedResource(
	//	&heapProps,
	//	D3D12_HEAP_FLAG_NONE,
	//	&bufferDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&mVertexBuffer))))
	//{
	//	throw std::runtime_error("Failed to create vertex buffer.");
	//}
	//UINT8* pVertexDataBegin;
	//CD3DX12_RANGE readRange(0, 0);
	//if (FAILED(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))))
	//{
	//	throw std::runtime_error("Failed to map vertex buffer.");
	//}
	//memcpy(pVertexDataBegin, vertices, sizeof(vertices));
	//mVertexBuffer->Unmap(0, nullptr);
	//mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
	//mVertexBufferView.StrideInBytes = sizeof(Vertex);
	//mVertexBufferView.SizeInBytes = vertexBufferSize;
}

void D3D12Window::InitIndexBuffer() {
	const UINT indexBufferSize = sizeof(mGeometry.indices[0]) * mGeometry.indices.size();
	CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	if (FAILED(md3dDevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mIndexBuffer))))
	{
		throw std::runtime_error("Failed to create index buffer.");
	}
	UINT8* pIndexDataBegin;
	CD3DX12_RANGE readRange(0, 0);
	if (FAILED(mIndexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin))))
	{
		throw std::runtime_error("Failed to map index buffer.");
	}
	memcpy(pIndexDataBegin, mGeometry.indices.data(), indexBufferSize);
	mIndexBuffer->Unmap(0, nullptr);
	mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
	mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
	mIndexBufferView.SizeInBytes = indexBufferSize;

	//const UINT indexBufferSize = sizeof(indices);
	//D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
	//CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	//if (FAILED(md3dDevice->CreateCommittedResource(
	//	&heapProps,
	//	D3D12_HEAP_FLAG_NONE,
	//	&bufferDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&mIndexBuffer))))
	//{
	//	throw std::runtime_error("Failed to create index buffer.");
	//}
	//UINT8* pIndexDataBegin;
	//CD3DX12_RANGE readRange(0, 0);
	//if (FAILED(mIndexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin))))
	//{
	//	throw std::runtime_error("Failed to map index buffer.");
	//}
	//memcpy(pIndexDataBegin, indices, sizeof(indices));
	//mIndexBuffer->Unmap(0, nullptr);
	//mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
	//mIndexBufferView.SizeInBytes = indexBufferSize;
	//mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
}

void D3D12Window::InitShader() {
	// 12
	mVertexShader = CompileShader(L"../../../src/Moteur/color.hlsl", nullptr, "VS", "vs_5_0");
	mPixelShader = CompileShader(L"../../../src/Moteur/color.hlsl", nullptr, "PS", "ps_5_0"); //Path Color
}

void D3D12Window::InitRootSignature() {
	CD3DX12_ROOT_PARAMETER rootParameters[1];
	rootParameters[0].InitAsConstantBufferView(0);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC versionedRootSignatureDesc = {};
	versionedRootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
	versionedRootSignatureDesc.Desc_1_0 = rootSignatureDesc;

	HRESULT hr = D3D12SerializeVersionedRootSignature(&versionedRootSignatureDesc, &mSignature, &mError);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to serialize root signature.");
	}

	hr = md3dDevice->CreateRootSignature(0, mSignature->GetBufferPointer(), mSignature->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create root signature.");
	}
}

void D3D12Window::InitPipeline() {


	if (mVertexShader == nullptr || mPixelShader == nullptr) {
		// Handle the error, e.g., log and return
		//throw std::runtime_error("Shaders not compiled correctly.");
		return;
	}
	// 14. Describe and create the PSO
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = mRootSignature;
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mVertexShader->GetBufferPointer()),
		mVertexShader->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mPixelShader->GetBufferPointer()),
		mPixelShader->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = 1;

	if (FAILED(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState))))
	{
		throw std::runtime_error("Failed to create pipeline state.");
	}
}

D3D12Window* D3D12Window::Get() {
	D3D12Window d3d12Window;
	return &d3d12Window;
}

void D3D12Window::InitD3D12() {
	InitDevice();
	InitFence();
	InitMSAA();
	InitCommand();
	InitSwapChain();
	InitDescriptorHeaps();
	InitRTV();
	InitDSV();
	InitViewport();
	InitConstantBuffer();
	InitVertexBuffer();
	InitIndexBuffer();
	InitShader();
	InitRootSignature();
	InitPipeline();
	InitCamera(); // Initialiser la camera
}

void D3D12Window::Draw(float dt) {
	UpdateCamera();

	mDirectCmdListAlloc->Reset();
	mCommandList->Reset(mDirectCmdListAlloc, mPipelineState);

	mCommandList->SetGraphicsRootSignature(mRootSignature);
	mCommandList->RSSetViewports(1, &mViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mCommandList->ResourceBarrier(1, &barrier);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRtvDescriptorSize);
	mCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	const float clearColor[] = { 0.8f, 0.8f, 0.9f, 1.0f };
	mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	mCommandList->IASetIndexBuffer(&mIndexBufferView);

	mWorldMatrix = XMMatrixRotationY(dt);

	cameraComponent->GetCamera()->UpdateViewMatrix();

	XMMATRIX wvpMatrix = cameraComponent->GetViewMatrix() * cameraComponent->GetProjectionMatrix();
	mCbData.mMvpMatrix = XMMatrixTranspose(wvpMatrix);

	void* pData;
	D3D12_RANGE readRange = { 0, 0 };
	mConstantBuffers[0]->Map(0, &readRange, reinterpret_cast<void**>(&pData));
	memcpy(pData, &mCbData, sizeof(mCbData));
	mConstantBuffers[0]->Unmap(0, nullptr);

	mCommandList->SetGraphicsRootConstantBufferView(0, mConstantBuffers[0]->GetGPUVirtualAddress());
	mCommandList->DrawIndexedInstanced(mGeometry.indices.size(), 1, 0, 0, 0);

	barrier = CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	mCommandList->ResourceBarrier(1, &barrier);

	if (FAILED(mCommandList->Close()))
	{
		throw std::runtime_error("Failed to close command list.");
	}

	ID3D12CommandList* ppCommandLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	const UINT64 currentFenceValue = mFenceValue;
	if (FAILED(mCommandQueue->Signal(mFence, currentFenceValue)))
	{
		throw std::runtime_error("Failed to signal command queue.");
	}
	mFenceValue++;

	if (FAILED(mSwapChain->Present(1, 0)))
	{
		throw std::runtime_error("Failed to present swap chain.");
	}

	if (mFence->GetCompletedValue() < currentFenceValue)
	{
		if (FAILED(mFence->SetEventOnCompletion(currentFenceValue, mFenceEvent)))
		{
			throw std::runtime_error("Failed to set fence event.");
		}
		WaitForSingleObject(mFenceEvent, INFINITE);
	}

	mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
}

void D3D12Window::CleanUp(){
	//Code de nettoyage des ressources Direct3D 12

// Attendre que le GPU ait termine avant de liberer les ressources
    if (mFence && mCommandQueue)
    {
        // Wait for the GPU to be done with all resources.
		mCommandQueue->Signal(mFence, mFenceValue);
        if (mFence->GetCompletedValue() < mFenceValue)
        {
			mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);
            WaitForSingleObject(mFenceEvent, INFINITE);
        }
    }
	CloseHandle(mFenceEvent);

	if (mPipelineState) mPipelineState->Release();
	if (mRootSignature) mRootSignature->Release();

	for (int i = 0; i < 2; ++i)	{
		if (mConstantBuffers[i]) mConstantBuffers[i]->Release();
	}

}

void D3D12Window::InitCamera() {
	systemCamera = new SystemCamera();
	Entity* cameraEntity = new Entity(99);
	cameraComponent = cameraEntity->AddComponent<ComponentCamera>();
	systemCamera->AddEntity(cameraEntity);
}

void D3D12Window::UpdateCamera() {
	if (cameraComponent) {/*
		XMMATRIX worldMatrix = XMMatrixIdentity(); */
		XMMATRIX viewMatrix = cameraComponent->GetViewMatrix();
		XMMATRIX projMatrix = cameraComponent->GetProjectionMatrix();

		//XMMATRIX wvpMatrix = worldMatrix * viewMatrix * projMatrix;

		//mCbData.mMvpMatrix = XMMatrixTranspose(wvpMatrix); 
	}
}



