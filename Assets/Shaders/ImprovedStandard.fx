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

cbuffer MaterialBuffer : register(b2)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float4 materialEmissive;
    float materialPower = 0.0f;
}

cbuffer SettingsBuffer : register(b3)
{
    bool useDiffuseMap;
    bool useNormalMap;
    bool useDisplacementMap;
    bool useSpecularMap;
    bool useShadowMap;
    float depthBias;
}

cbuffer ExtraSettingsBuffer : register(b4)
{
	float brightness;
	float saturation;
	float contrast;
    bool useExtraSettings;
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); // Same sampler is fine
Texture2D displacementMap : register(t2); // Same sampler is fine
Texture2D specularMap : register(t3); // Same sampler is fine
Texture2D shadowMap : register(t4);

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

VS_OUTPUT VS(VS_INPUT input)
{
	// Start point for week3
    VS_OUTPUT output;

    matrix toWorld = world;
    matrix toNDC = wvp[0];
	
    float3 localPosition = input.position;
    if (useDisplacementMap)
    {
        float displacementColor = displacementMap.SampleLevel(textureSampler, input.texCoord, 0.0f).r;
        localPosition += (input.normal * displacementColor * displacementWeight);
    }
	
    output.position = mul(float4(localPosition, 1.0f), toNDC);
    output.worldNormal = mul(input.normal, (float3x3) toWorld);
    output.worldTangent = mul(input.tangent, (float3x3) toWorld);
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - mul(float4(localPosition, 1.0f), world).xyz);
    output.texCoord = input.texCoord;
	
    if (useShadowMap)
    {
        output.lightNDCPosition = mul(float4(localPosition, 1.0f), wvp[1]);
    }
    
    return output;
}

float3 RGBtoHSL(float3 color)
{
	float minval = min(min(color.r, color.g), color.b);
	float maxval = max(max(color.r, color.g), color.b);
	float delta = maxval - minval;
	float h = 0, s = 0, l = (maxval + minval) / 2;

	if (delta != 0)
	{
		s = delta / (1 - abs(2 * l - 1));
		if (maxval == color.r)
		{
			h = 60 * fmod(((color.g - color.b) / delta), 6);
		}
		else if (maxval == color.g)
		{
			h = 60 * (((color.b - color.r) / delta) + 2);
		}
		else
		{
			h = 60 * (((color.r - color.g) / delta) + 4);
		}
	}
	return float3(h, s, l);
}

float3 HSLtoRGB(float3 color)
{
	float c = (1 - abs(2 * color.z - 1)) * color.y;
	float x = c * (1 - abs(fmod((color.x / 60), 2) - 1));
	float m = color.z - c / 2;

	float3 rgb;
	if (color.x < 60)
	{
		rgb = float3(c, x, 0);
	}
	else if (color.x < 120)
	{
		rgb = float3(x, c, 0);
	}
	else if (color.x < 180)
	{
		rgb = float3(0, c, x);
	}
	else if (color.x < 240)
	{
		rgb = float3(0, x, c);
	}
	else if (color.x < 300)
	{
		rgb = float3(x, 0, c);
	}
	else
	{
		rgb = float3(c, 0, x);
	}

	return rgb + m;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
	
    if (useNormalMap)
    {
        float3 t = normalize(input.worldTangent);
        float3 b = normalize(cross(n, t));
        float3x3 tbnw = float3x3(t, b, n);
	
        float4 normalMapColor = normalMap.Sample(textureSampler, input.texCoord);
        float3 unpackedNormalMap = normalize(float3((normalMapColor.xy * 2.0f) - 1.0f, normalMapColor.z));
        n = normalize(mul(unpackedNormalMap, tbnw));
    }
	
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
	
    float4 ambient = lightAmbient * materialAmbient;
	
    float d = saturate(dot(light, n));
    float4 diffuse = d * lightDiffuse * materialDiffuse;
	
    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, materialPower);
    float4 specular = s * lightSpecular * materialSpecular;
	
    float4 diffuseMapColor = (useDiffuseMap) ? diffuseMap.Sample(textureSampler, input.texCoord) : 1;
    float4 specularMapColor = (useSpecularMap) ? specularMap.Sample(textureSampler, input.texCoord).r : 1.0f;
	
    float4 finalColor = (ambient + diffuse + materialEmissive) * diffuseMapColor + (specular * specularMapColor);
	
    if (useShadowMap)
    {
        float actualDepth = 1.0f - (input.lightNDCPosition.z / input.lightNDCPosition.w);
        float2 shadowUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
        float u = (shadowUV.x + 1.0f) * 0.5f;
        float v = 1.0f - (shadowUV.y + 1.0f) * 0.5f;
        
        if (saturate(u) == u && saturate(v) == v)
        {
            float4 savedColor = shadowMap.Sample(textureSampler, float2(u, v));
            float savedDepth = savedColor.r;
            
            if (savedDepth > actualDepth + depthBias)
            {
                finalColor = (ambient + materialEmissive) * diffuseMapColor;
            }
        }
    }
    
    if (useExtraSettings)
    {
      // Apply brightness adjustment
        float3 rgb = finalColor.rgb * brightness;
        rgb = clamp(rgb, 0.0, 1.0); // clamp the color to ensure it stays in the valid range
        finalColor.rgb = rgb;
        
        // Apply saturation adjustment
        float3 hsl = RGBtoHSL(finalColor.rgb);
        hsl.y += saturation;
        finalColor.rgb = HSLtoRGB(hsl);
        
        // Apply contrast adjustment
	    finalColor.rgb = (finalColor.rgb - 0.5) * contrast + 0.5;
        finalColor.rgb = clamp(finalColor.rgb, 0.0, 1.0);
    }   
	
    return finalColor;
}