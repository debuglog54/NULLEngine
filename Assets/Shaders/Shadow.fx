cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uvCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 postion : SV_Position;
    float4 lightNDCPosition : TEXCOORD0;

};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.postion = mul(float4(input.position, 1), wvp);
    output.lightNDCPosition = mul(float4(input.position, 1), wvp);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float depth = 1.0f - input.lightNDCPosition.z / input.lightNDCPosition.w;
    return float4(depth, 1.0f, 1.0f, 1.0f);
}