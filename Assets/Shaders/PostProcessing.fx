cbuffer PostPorcessingBuffer : register(b0)
{
	int mode;
	float param0;
	float param1;
	float param2;
}

Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
Texture2D textureMap2 : register(t2);
Texture2D textureMap3 : register(t3);
SamplerState textureSampler : register(s0);

struct VS_INPUT // Can be VertexInputShader
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 postion : SV_Position;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.postion = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 finalColor;
	
	if (mode == 0) //None
	{
		finalColor = textureMap0.Sample(textureSampler, input.texCoord);
	}
	else if (mode == 1) // Monochrome
	{
		float4 color = textureMap0.Sample(textureSampler, input.texCoord);
		finalColor = (color.r + color.g + color.b) / 3.0f;
	}
	else if (mode == 2) // Invert
	{
		float4 color = textureMap0.Sample(textureSampler, input.texCoord);
		finalColor = 1.0f - color;
	}
	else if (mode == 3) // Mirror
	{
		float2 texCoord = input.texCoord;
		texCoord.y *= param0;
		finalColor = textureMap0.Sample(textureSampler, texCoord);
	}
	else if (mode == 4) // Blur
	{
		float u = input.texCoord.x;
		float v = input.texCoord.y;
		
		finalColor = textureMap0.Sample(textureSampler, float2(u, v))
		+ textureMap0.Sample(textureSampler, float2(u + param0, v))
		+ textureMap0.Sample(textureSampler, float2(u - param0, v))
		+ textureMap0.Sample(textureSampler, float2(u, v + param1))
		+ textureMap0.Sample(textureSampler, float2(u, v - param1))
		+ textureMap0.Sample(textureSampler, float2(u + param0, v + param1))
		+ textureMap0.Sample(textureSampler, float2(u + param0, v - param1))
		+ textureMap0.Sample(textureSampler, float2(u - param0, v + param1))
		+ textureMap0.Sample(textureSampler, float2(u - param0, v - param1));
		finalColor *= 0.12f;
	}
	else if (mode == 5) // Combine2
	{
		float4 color0 = textureMap0.Sample(textureSampler, input.texCoord);
		float4 color1 = textureMap1.Sample(textureSampler, input.texCoord);
		finalColor = color0 + color1;
	}
	
	return finalColor;
}
