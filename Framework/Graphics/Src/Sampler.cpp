#include "Precompiled.h"
#include "Sampler.h"
#include "GraphicsSystem.h"

using namespace NULLEngine::Graphics;

namespace NULLEngine::Graphics
{
	D3D11_FILTER GetFilter(Sampler::Filter filter)
	{
		switch (filter)
		{
		case NULLEngine::Graphics::Sampler::Filter::Point:  return D3D11_FILTER_MIN_MAG_MIP_POINT;

		case NULLEngine::Graphics::Sampler::Filter::Linear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		case NULLEngine::Graphics::Sampler::Filter::Anisotropic: return D3D11_FILTER_ANISOTROPIC;
		}

		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}

	D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(Sampler::AdressMode addressMode)
	{
		switch (addressMode)
		{
		case NULLEngine::Graphics::Sampler::AdressMode::Border: return D3D11_TEXTURE_ADDRESS_BORDER;

		case NULLEngine::Graphics::Sampler::AdressMode::Clamp:return D3D11_TEXTURE_ADDRESS_CLAMP;

		case NULLEngine::Graphics::Sampler::AdressMode::Mirror:return D3D11_TEXTURE_ADDRESS_MIRROR;

		case NULLEngine::Graphics::Sampler::AdressMode::Wrap:return D3D11_TEXTURE_ADDRESS_WRAP;
		}

		return D3D11_TEXTURE_ADDRESS_BORDER;
	}

	Sampler::~Sampler()
	{
		ASSERT((mSampler == nullptr), "Sampler was released");
	}

	void Sampler::Terminate()
	{
		SafeRelease(mSampler);
	}

	void Sampler::Initialize(Filter filter, AdressMode adressMode)
	{
		auto d3dFilter = GetFilter(filter);
		auto d3dAddressMode = GetAddressMode(adressMode);

		D3D11_SAMPLER_DESC desc{};
		desc.Filter = d3dFilter;
		desc.AddressU = d3dAddressMode;
		desc.AddressV = d3dAddressMode;
		desc.AddressW = d3dAddressMode;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		auto device = GraphicsSystem::Get()->GetDevice();
		auto hr = device->CreateSamplerState(&desc, &mSampler);
		ASSERT(SUCCEEDED(hr), "Failed to create sampler state");
	}


	void Sampler::BindVS(uint32_t slot) const
	{
		auto context = GraphicsSystem::Get()->GetContext();
		context->VSSetSamplers(slot, 1, &mSampler);
	}

	void Sampler::BindPS(uint32_t slot) const
	{
		auto context = GraphicsSystem::Get()->GetContext();
		context->PSSetSamplers(slot, 1, &mSampler);
	}
}