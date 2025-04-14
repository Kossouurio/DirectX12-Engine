#pragma once
#include <d3dcompiler.h>

class D3D12Shader {
public:
    D3D12Shader();
    ~D3D12Shader();

    // Compilation des shaders
    bool CompileVertexShader(
        const std::wstring& filename,
        const D3D_SHADER_MACRO* defines,
        const std::string& entrypoint,
        const std::string& target);

    bool CompilePixelShader(
        const std::wstring& filename,
        const D3D_SHADER_MACRO* defines,
        const std::string& entrypoint,
        const std::string& target);

    // Accesseurs
    ID3DBlob* GetVertexShaderBlob() const { return mVertexShaderBlob; }
    ID3DBlob* GetPixelShaderBlob() const { return mPixelShaderBlob; }

    // Methode utilitaire
    static ID3DBlob* CompileShader(
        const wstring& filename,
        const D3D_SHADER_MACRO* defines,
        const string& entrypoint,
        const string& target);

private:
    ID3DBlob* mVertexShaderBlob = nullptr;
    ID3DBlob* mPixelShaderBlob = nullptr;
};