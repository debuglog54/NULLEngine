// Description: Simple shader that does transform.
// b - buffer
// c - buffer offset
// t - texture
// s - sampler 
// u - unoreder

cbuffer TransformBuffer : register(b0)
{
    matrix world;
    matrix wvp[2];
    float3 viewPosition;
    float displacementWeight;
}

cbuffer LightBuffer : register(b1)
{
    float3 lightDirection;
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
}

cbuffer SettingsBuffer : register(b3)
{
    float4 color1;
    float4 color2;
    float4 borderColor;
    float blendingCoeficient;
    int useItensity;
    int useDiffuseAsSecondColor;
    float padding[1];
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); // Same sampler is fine

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView : TEXCOORD1;
    float2 texCoord : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
};

static matrix Identity =
{
    1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

float OutlineThreshold = .3f;
float4 LineColor = float4(0, 0, 0, 1);
float LineThickness = .03;


float4 mixColor = float4(1.0, 0.0, 0.0, 1.0); // a red color
float blendAmount = 0.5f;

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    matrix toWorld = world;
    matrix toNDC = wvp[0];
 
    output.position = mul(float4(input.position, 1), toNDC);
    output.worldNormal = normalize(mul(input.normal, (float3x3) toWorld));
    output.worldTangent = mul(input.tangent, (float3x3) toWorld);
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - mul(float4(input.position, 1.0f), world).xyz);
    output.texCoord = input.texCoord;
	
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
	
   // float dotProduct = dot(input.worldNormal, normalize(lightDirection));
    float dotProduct = dot(-light, normalize(lightDirection));
    float intensity;
    
    float shading = step(0.9, dotProduct) + step(0.6, dotProduct) + step(0.3, dotProduct);
    float4 finalColor;
    
    float4 firstColor = shading * color1;
    float4 secondColor = shading * color2;
    
    if (useDiffuseAsSecondColor)
    {
            float4 diffuseMapColor = diffuseMap.Sample(textureSampler, input.texCoord);
        if (useItensity)
        {
            intensity = dot(normalize(lightDirection), input.worldNormal);
            if (intensity < 0)
                intensity = 0;
 
      // Discretize the intensity, based on a few cutoff points
            if (intensity > 0.95)
                diffuseMapColor = float4(1.0, 1, 1, 1.0) * diffuseMapColor;
            else if (intensity > 0.5)
                diffuseMapColor = float4(0.7, 0.7, 0.7, 1.0) * diffuseMapColor;
            else if (intensity > 0.05)
                diffuseMapColor = float4(0.35, 0.35, 0.35, 1.0) * diffuseMapColor;
            else
                diffuseMapColor = float4(0.1, 0.1, 0.1, 1.0) * diffuseMapColor;
        }
      
        secondColor = shading*diffuseMapColor;
    }

    // Determine the shading level based on the dot product
    float4 mainColor = lerp(firstColor, secondColor, blendingCoeficient);
    finalColor = mainColor;
    
      // Edge detection
    float edgeThreshold = 0.1;
    float edgeFactor = 1.0 - smoothstep(0.0, edgeThreshold, dotProduct);
    float edgeWidth = 0.05;
    float4 edgeColor = lightSpecular;
    float4 outlineColor = edgeFactor * edgeColor;

// Apply outline
    float outlineThreshold = 0;
    float outline = step(outlineThreshold, edgeFactor);

    if (useItensity)
    {
        intensity = dot(normalize(lightDirection), input.worldNormal);
        if (intensity < 0)
            intensity = 0;
 
      // Discretize the intensity, based on a few cutoff points
        if (intensity > 0.95)
            finalColor = float4(1.0, 1, 1, 1.0) * finalColor;
        else if (intensity > 0.5)
            finalColor = float4(0.7, 0.7, 0.7, 1.0) * finalColor;
        else if (intensity > 0.05)
            finalColor = float4(0.35, 0.35, 0.35, 1.0) * finalColor;
        else
            finalColor = float4(0.1, 0.1, 0.1, 1.0) * finalColor;
    }
    
    finalColor += outline * outlineColor;
    return finalColor;
}





