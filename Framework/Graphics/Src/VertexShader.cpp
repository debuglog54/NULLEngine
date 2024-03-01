#include "Precompiled.h"
#include "VertexShader.h"
#include "VertexTypes.h"
#include <Graphics/Inc/GraphicsSystem.h>

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

namespace
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexLayout(uint32_t vertexFormat)
    {

        std::vector<D3D11_INPUT_ELEMENT_DESC> desc;

        if (vertexFormat & VE_Position)
        {
            desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        }
        if (vertexFormat & VE_Normal)
        {
            desc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        }
        if (vertexFormat & VE_Tangent)
        {
            desc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        }
        if (vertexFormat & VE_Color)
        {
            desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        }
        if (vertexFormat & VE_TexCoord)
        {
            desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        }
        if (vertexFormat & VE_BlendIndex)
        {
            desc.push_back({ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        }
        if (vertexFormat & VE_BlendWeight)
        {
            desc.push_back({ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        }

        return desc;
    }
}

void VertexShader::Initiliaze(const std::filesystem::path& filePath, uint32_t format)
{
    auto device = GraphicsSystem::Get()->GetDevice();

    // Create Vertex Buffer
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(
        filePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS", "vs_5_0",
        shaderFlags, 0, &shaderBlob, &errorBlob);

    if (errorBlob && errorBlob->GetBufferPointer())
    {
        LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));

    }
    ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader");

    hr = device->CreateVertexShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mVertexShader);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");

    //===================================================================================
    // Create the Input
    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout = GetVertexLayout(format);
  // vertexLayout.push_back({ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT });
  // vertexLayout.push_back({ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT });

    hr = device->CreateInputLayout(
        vertexLayout.data(),
        (UINT)vertexLayout.size(),
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        &mInputLayout);

    ASSERT(SUCCEEDED(hr), "Failed to create input layout");
    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);
}

void VertexShader::Terminate()
{
    SafeRelease(mVertexShader);
    SafeRelease(mInputLayout);
}

void VertexShader::Bind()
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->VSSetShader(mVertexShader, nullptr, 0);
    context->IASetInputLayout(mInputLayout);
}
