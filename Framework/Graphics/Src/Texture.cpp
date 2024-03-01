#include "Precompiled.h"
#include "Texture.h"

#include"GraphicsSystem.h"
#include <DirectXTK-main/Inc/WICTextureLoader.h>

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

void Texture::UnbindPS(uint32_t slot)
{
    static ID3D11ShaderResourceView* dummy = nullptr;
    GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}

Texture:: ~Texture()
{
    ASSERT((mShaderSourceView == nullptr), "Shader Resource View was not destroyed!");
}


Texture::Texture(Texture&& rhs)  noexcept
    : mShaderSourceView(rhs.mShaderSourceView)
{
    rhs.mShaderSourceView = nullptr;
}

Texture& Texture::operator=( Texture&& rhs) noexcept
{
    mShaderSourceView = rhs.mShaderSourceView;
    rhs.mShaderSourceView = nullptr;
    return *this;
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
    auto device = GraphicsSystem::Get()->GetDevice();
    auto context = GraphicsSystem::Get()->GetContext();
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderSourceView);
    ASSERT(SUCCEEDED(hr), "Failed to load texture");
}

void Texture::Initialize(uint32_t width, uint32_t height, Format format)
{
    ASSERT(false, "Not Implemented Yet");
}

void Texture::Terminate()
{
    SafeRelease(mShaderSourceView);
}

void Texture::BindVS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->VSSetShaderResources(slot, 1, &mShaderSourceView);
}

void Texture::BindPS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->PSSetShaderResources(slot, 1, &mShaderSourceView);
}


DXGI_FORMAT Texture::GetDXGIFormat(Format format)
{
    switch (format)
    {
    case Texture::Format::RGBA_U8:
        return DXGI_FORMAT_R8G8B8A8_UNORM;

    case Texture::Format::RGBA_U32:
        return DXGI_FORMAT_R32G32B32A32_UINT;

    default:
        break;

        return DXGI_FORMAT_R8G8B8A8_UNORM;
    }
}