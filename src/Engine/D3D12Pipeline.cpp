#include "pch.h"
#include "D3D12Pipeline.h"


D3D12Pipeline::D3D12Pipeline(ID3D12Device* device) : mDevice(device) {
}

D3D12Pipeline::~D3D12Pipeline() {
    if (mPipelineState) mPipelineState->Release();
}

bool D3D12Pipeline::Create(
    ID3D12RootSignature* rootSignature,
    ID3DBlob* vertexShader,
    ID3DBlob* pixelShader,
    DXGI_FORMAT rtvFormat,
    bool depthEnabled,
    DXGI_FORMAT dsvFormat,
    bool enableBlending)
{
    if (!rootSignature || !vertexShader || !pixelShader) {
        return false;
    }

    // Description du pipeline state
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

    // Definition du layout d'entree
    psoDesc.InputLayout = { mInputLayout.data(), (UINT)(mInputLayout.size()) };

    // Liaison avec la signature racine
    psoDesc.pRootSignature = rootSignature;

    // Configuration des shaders
    psoDesc.VS = {
        reinterpret_cast<BYTE*>(vertexShader->GetBufferPointer()),
        vertexShader->GetBufferSize()
    };

    psoDesc.PS = {
        reinterpret_cast<BYTE*>(pixelShader->GetBufferPointer()),
        pixelShader->GetBufferSize()
    };

    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);


    // Configuration de l'etat de melange
    if (enableBlending) {
        psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
        psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
        psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
        psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
        psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
        psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
        psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    }
    else {
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    }

    // Configuration de l'etat de profondeur/stencil
    if (depthEnabled) {
        psoDesc.DepthStencilState.DepthEnable = TRUE;
        psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.DSVFormat = dsvFormat;
    }
    else {
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
    }

    // Configuration du masque d'echantillonnage
    psoDesc.SampleMask = UINT_MAX;

    // Configuration du type de topologie primitive
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // Configuration du format de render target
    psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = rtvFormat;

    // Configuration de l'echantillonnage
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;

    // Creation du pipeline state object
    HRESULT hr = mDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState));

    return SUCCEEDED(hr);

}
void D3D12Pipeline::AddInputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElements) {
    mInputLayout = inputElements;
}