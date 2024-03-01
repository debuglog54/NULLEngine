#include "GameState.h"
#include "Input/Inc/InputSystem.h"

void GameState::Initialize()
{
	// NDC - Normalized Device Coordinate
   //
   //            +-----------------+
   //           /                 /|
   //     1.0  +-----------------+ |
   //          |        ^        | |
   //          |        |        | |
   //          | <------+------> | |
   //          |        |        | | 1.0
   //          |        v        |/
   //    -1.0  +-----------------+ 0.0
   //      -1.0               1.0
   //

	NULLEngine::Graphics::GraphicsSystem::Get()->SetClearColor(Colors::Black);
	OnCreateShape();
	auto device = GraphicsSystem::Get()->GetDevice();
	//===================================================================================
	// Create Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(mVertecies.size() * sizeof(Vertex));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVertecies.data();
	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");

	//===================================================================================
   // Create Vertex Buffer
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
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
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT });
	vertexLayout.push_back({ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT });


	hr = device->CreateInputLayout(
		vertexLayout.data(),
		(UINT)vertexLayout.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);

	ASSERT(SUCCEEDED(hr), "Failed to create input layout");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	//===================================================================================
	// Create the Pixe Shader
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);

	if (errorBlob && errorBlob->GetBufferPointer())
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));

	}
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);

	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void GameState::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = Input::InputSystem::Get();

	if (inputSystem->IsKeyPressed(Input::KeyCode::F1))
	{
		NULLEngine::Graphics::GraphicsSystem::Get()->SetClearColor(Colors::Red);
	}
	else if (inputSystem->IsKeyPressed(Input::KeyCode::F2))
	{
		NULLEngine::Graphics::GraphicsSystem::Get()->SetClearColor(Colors::Green);
	}
}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetShader(mVertexShader, nullptr, 0);
	context->IASetInputLayout(mInputLayout);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->Draw((UINT)mVertecies.size(), 0);
}

void GameState::DebugUI()
{
}

void GameState::OnCreateShape()
{
	mVertecies.push_back({ EMath::Vector3(0.0f,0.5f,0.0f), Colors::Yellow });
	mVertecies.push_back({ EMath::Vector3(0.5f,-0.5f,0.0f), Colors::Red });
	mVertecies.push_back({ EMath::Vector3(-0.5f,-0.5f,0.0f), Colors::Green });
}