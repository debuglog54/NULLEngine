#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

using namespace NULLEngine::Graphics;

ConstantBuffer::~ConstantBuffer()
{
	ASSERT((mConstantBuffer == nullptr), "Buffer not released!");
}

void ConstantBuffer::Initialize(uint32_t bufferSize)
{
	D3D11_BUFFER_DESC cbufferDesc{};
	cbufferDesc.ByteWidth = bufferSize;
	cbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbufferDesc.CPUAccessFlags = 0;
	cbufferDesc.MiscFlags = 0;
	cbufferDesc.StructureByteStride = 0;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBuffer(&cbufferDesc, nullptr, &mConstantBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}

void ConstantBuffer::Update(const void* data) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}