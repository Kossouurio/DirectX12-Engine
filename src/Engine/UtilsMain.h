// Core System Headers
#include "System.h"
#include "SystemManager.h"
#include "SystemAudio.h"
#include "SystemCamera.h"
#include "SystemCollider.h"
#include "SystemPhysics.h"
#include "SystemRender.h"
#include "SystemTransform.h"

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
#include "ComponentUI.h"

// Entity Management
#include "Entity.h"
#include "EntityManager.h"

// Game Engine and Utilities
#include "GameEngine.h"
#include "GameTimer.h"
#include "Geometry.h"
#include "MathHelper.h"
#include "Mesh.h"
#include "Input.h"
#include "Logs.h"
#include "Timer.h"
#include "Vector.h"

// Script Management
#include "Script.h"
#include "ScriptManager.h"
#include "ScriptPlayerController.h"

// Events
#include "SEvent.h"

// Rendering and Graphics
#include "D3D12Window.h"
#include "d3dx12.h"
#include "Texture.h"

// Miscellaneous
#include "Camera.h"
#include "main.h"
#include "pch.h"
#include "ToolBox.h"
#include "UtilsImport.h"

#include <dxgi1_4.h>
#include <wincodec.h>
