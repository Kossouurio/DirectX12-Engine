#include "pch.h"
#include "D3D12RootSignature.h"

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers()
{
    // Applications usually only need a handful of samplers.  So just define them all up front
    // and keep them available as part of the root signature.  

    const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
        0, // shaderRegister
        D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

    const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
        1, // shaderRegister
        D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

    const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
        2, // shaderRegister
        D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

    const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
        3, // shaderRegister
        D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

    const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
        4, // shaderRegister
        D3D12_FILTER_ANISOTROPIC, // filter
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
        0.0f,                             // mipLODBias
        8);                               // maxAnisotropy

    const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
        5, // shaderRegister
        D3D12_FILTER_ANISOTROPIC, // filter
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
        0.0f,                              // mipLODBias
        8);                                // maxAnisotropy

    return {
        pointWrap, pointClamp,
        linearWrap, linearClamp,
        anisotropicWrap, anisotropicClamp };
}

D3D12RootSignature::D3D12RootSignature(ID3D12Device* device) : mDevice(device) {
}

D3D12RootSignature::~D3D12RootSignature() {
    if (mRootSignature) mRootSignature->Release();
    if (mSignatureBlob) mSignatureBlob->Release();
    if (mErrorBlob) mErrorBlob->Release();
}

bool D3D12RootSignature::Create() {
    // **Ajout du constant buffer attendu par le shader**
    D3D12_ROOT_DESCRIPTOR rootCBVDescriptor = {};
    rootCBVDescriptor.ShaderRegister = 0; // Correspond a `b0` dans le shader
    rootCBVDescriptor.RegisterSpace = 0;

    D3D12_ROOT_PARAMETER rootParameters[1] = {};
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[0].Descriptor = rootCBVDescriptor;
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    // **Creation de la Root Signature avec les parametres**
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = _countof(rootParameters);
    rootSignatureDesc.pParameters = rootParameters;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // **Serialisation et creation de la Root Signature**
    HRESULT hr = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &mSignatureBlob,
        &mErrorBlob
    );

    if (FAILED(hr)) {
        if (mErrorBlob) {
            OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
            mErrorBlob->Release();
        }
        return false;
    }

    hr = mDevice->CreateRootSignature(
        0,
        mSignatureBlob->GetBufferPointer(),
        mSignatureBlob->GetBufferSize(),
        IID_PPV_ARGS(&mRootSignature)
    );

    return SUCCEEDED(hr);
}


bool D3D12RootSignature::CreateWithDescriptors() {
    // Creation d'un descripteur racine pour un buffer constant
    D3D12_ROOT_DESCRIPTOR rootCBVDescriptor;
    rootCBVDescriptor.RegisterSpace = 0;
    rootCBVDescriptor.ShaderRegister = 0;

    // Creation d'une plage de descripteurs pour les textures
    D3D12_DESCRIPTOR_RANGE descriptorTableRanges[1];
    descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorTableRanges[0].NumDescriptors = 1;
    descriptorTableRanges[0].BaseShaderRegister = 0;
    descriptorTableRanges[0].RegisterSpace = 0;
    descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // Creation d'une table de descripteurs
    D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
    descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges);
    descriptorTable.pDescriptorRanges = descriptorTableRanges;

    // Creation des parametres racine
    D3D12_ROOT_PARAMETER rootParameters[2];

    // Parametre 0 : Un buffer constant pour les matrices
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[0].Descriptor = rootCBVDescriptor;
    // Changer ici de VERTEX a ALL pour rendre le constant buffer accessible a tous les shaders
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    // Parametre 1 : Une table de descripteurs pour les textures
    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameters[1].DescriptorTable = descriptorTable;
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    auto SamplerDesc = GetStaticSamplers();

    // Description de la signature racine
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(
        _countof(rootParameters),
        rootParameters,
        SamplerDesc.size(),
        SamplerDesc.data(),
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
        // Supprimer cette ligne qui bloquait l'acces au geometry shader
        // D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
    );

    // Serialisation de la signature racine
    HRESULT hr = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &mSignatureBlob,
        &mErrorBlob
    );

    if (FAILED(hr)) {
        if (mErrorBlob) {
            OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
        }
        return false;
    }

    // Creation de la signature racine
    hr = mDevice->CreateRootSignature(
        0,
        mSignatureBlob->GetBufferPointer(),
        mSignatureBlob->GetBufferSize(),
        IID_PPV_ARGS(&mRootSignature)
    );

    return SUCCEEDED(hr);
}
bool D3D12RootSignature::CreateWithLighting() {
    // Creation de descripteurs racine pour les buffers constants
    D3D12_ROOT_DESCRIPTOR rootCBVDescriptors[2];

    // CBV0: Matrices de transformation (register b0)
    rootCBVDescriptors[0].RegisterSpace = 0;
    rootCBVDescriptors[0].ShaderRegister = 0;

    // CBV1: Donnees de lumiere (register b1)
    rootCBVDescriptors[1].RegisterSpace = 0;
    rootCBVDescriptors[1].ShaderRegister = 1;

    // Creation d'une plage de descripteurs pour les textures
    D3D12_DESCRIPTOR_RANGE descriptorTableRanges[1];
    descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorTableRanges[0].NumDescriptors = 1;
    descriptorTableRanges[0].BaseShaderRegister = 0;
    descriptorTableRanges[0].RegisterSpace = 0;
    descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // Creation d'une table de descripteurs
    D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
    descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges);
    descriptorTable.pDescriptorRanges = descriptorTableRanges;

    // Creation des parametres racine
    D3D12_ROOT_PARAMETER rootParameters[3];

    // Parametre 0 : Un buffer constant pour les matrices et les transformations
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[0].Descriptor = rootCBVDescriptors[0];
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    // Parametre 1 : Un buffer constant pour les donnees de lumiere
    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[1].Descriptor = rootCBVDescriptors[1];
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    // Parametre 2 : Une table de descripteurs pour les textures
    rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameters[2].DescriptorTable = descriptorTable;
    rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	auto SamplerDesc = GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
		_countof(rootParameters),
		rootParameters,
		SamplerDesc.size(),
		SamplerDesc.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);


    // Serialisation de la signature racine
    HRESULT hr = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &mSignatureBlob,
        &mErrorBlob
    );

    if (FAILED(hr)) {
        if (mErrorBlob) {
            OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
        }
        return false;
    }

    // Creation de la signature racine
    hr = mDevice->CreateRootSignature(
        0,
        mSignatureBlob->GetBufferPointer(),
        mSignatureBlob->GetBufferSize(),
        IID_PPV_ARGS(&mRootSignature)
    );

    return SUCCEEDED(hr);
    }

