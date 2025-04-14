#pragma once

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "Winmm.lib")

#define KEY_LEFT  37 
#define KEY_RIGHT 39  
#define KEY_UP    38
#define KEY_DOWN  40

#define KEY_CTRL  17
#define KEY_SPACE 32 

#define KEY_Z	 90
#define KEY_Q	 81
#define KEY_S	 83
#define KEY_D	 68
#define KEY_E	 69
#define KEY_P	 80

#define LMB 0x01
#define RMB 0x02

// --- Libraries ---
#include <windows.h>

#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <d3d12.h> 
#include "d3dx12.h"
#include <wincodec.h>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <assert.h>
#include <list>
#include <map>
#include <memory>
#include <wrl.h>
#include <float.h>
#include <cmath>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <unordered_map>
#include <functional>
#include <chrono>

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include <DirectXPackedVector.h>
#include <array>
#include <cassert>
#include <cstdio>

// --- Libraries ---

using namespace DirectX;
using namespace std;

// --- Utils ---
//#include "Thread.h"
#include "ToolBox.h"
#include "Vector.h"
#include "Logs.h"
#include "Geometry.h"
#include "Input.h"
// --- Utils ---

#include "d3dx12.h"
#include "DDSTextureLoader.h"
#include "MathHelper.h"
#include "SEvent.h"

//#include "WindowConfig.h"

#include "Camera.h"
// Component Headers
#include "Component.h"
#include "ComponentAudio.h"
#include "ComponentCamera.h"
#include "ComponentCollider.h"
#include "ComponentHealth.h"
#include "ComponentInput.h"
#include "ComponentMesh.h"
#include "ComponentPhysics.h"
#include "ComponentRender.h"
#include "ComponentTransform.h"
#include "ComponentParticule.h"
#include "ComponentUI.h"
#include "ComponentScore.h"
#include "ComponentAI.h"

#include "Script.h"
#include "ScriptManager.h"
#include "D3D12Buffers.h"
#include "D3D12Shader.h"
#include "D3D12Texture.h"
#include "D3D12RootSignature.h"
#include "D3D12Pipeline.h"
#include "D3D12Light.h"
#include "Mesh.h"
#include "D3D12Window.h"

// Core System Headers
#include "System.h"
#include "SystemManager.h"
#include "SystemAudio.h"
#include "SystemScene.h"
#include "SystemCamera.h"
#include "SystemeParticule.h"
#include "SystemCollider.h"
#include "SystemPhysics.h"
#include "SystemRender.h"
#include "SystemTransform.h"
#include "SystemUI.h"
#include "SystemInput.h"
#include "SystemGameplay.h"
#include "SystemAI.h"

// Entity Management
#include "Entity.h"
#include "EntityManager.h"

// Game Engine and Utilities
#include "GameTimer.h"
#include "Geometry.h"
#include "Input.h"
#include "Logs.h"
#include "Vector.h"

// Script Management
#include "ScriptPlayerController.h"

// Events

// Rendering and Graphics
#include "Texture.h"
// Miscellaneous
#include "ToolBox.h"
#include "GameEngine.h"


#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif
