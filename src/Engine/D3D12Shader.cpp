#include "pch.h"
#include "D3D12Shader.h"

D3D12Shader::D3D12Shader() {
}

D3D12Shader::~D3D12Shader() {
    if (mVertexShaderBlob) mVertexShaderBlob->Release();
    if (mPixelShaderBlob) mPixelShaderBlob->Release();
}

bool D3D12Shader::CompileVertexShader(
    const wstring& filename,
    const D3D_SHADER_MACRO* defines,
    const string& entrypoint,
    const string& target)
{
    mVertexShaderBlob = CompileShader(filename, defines, entrypoint, target);
    return mVertexShaderBlob != nullptr;
}

bool D3D12Shader::CompilePixelShader(
    const wstring& filename,
    const D3D_SHADER_MACRO* defines,
    const string& entrypoint,
    const string& target)
{
    mPixelShaderBlob = CompileShader(filename, defines, entrypoint, target);
    return mPixelShaderBlob != nullptr;
}

ID3DBlob* D3D12Shader::CompileShader(
    const wstring& filename,
    const D3D_SHADER_MACRO* defines,
    const string& entrypoint,
    const string& target)
{
    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* byteCode = nullptr;
    ID3DBlob* errors = nullptr;
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(),
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entrypoint.c_str(),
        target.c_str(),
        compileFlags,
        0,
        &byteCode,
        &errors);

    if (FAILED(hr) && errors != nullptr) {
        OutputDebugStringA((char*)errors->GetBufferPointer());
        return nullptr;
    }
    if (errors) errors->Release();
    return byteCode;
}