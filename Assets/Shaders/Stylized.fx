// Stylized Effect
// Adds stylized rendering with outlines and normal expansion like Borderlands

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    matrix world;
    float3 viewPosition;
    float padding0;
}

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
    float padding1;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialEmissive;
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float materialShininess;
    float3 padding2;
}

cbuffer SettingsBuffer : register(b3)
{
    int useDiffuseMap;
    int useNormalMap;
    int useShadowMap;
    int renderOutline;
    float outlineWidth;
    float colorBands;
    float specularThreshold;
    float outlineThickness;
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
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
    float3 worldPosition : POSITION;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float2 texCoord : TEXCOORD;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
};

// Vertex Shader for normal rendering pass
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 worldPos = mul(float4(input.position, 1.0f), world);
    output.worldPosition = worldPos.xyz;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldNormal = mul(input.normal, (float3x3) world);
    output.worldTangent = mul(input.tangent, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - worldPos.xyz);
    output.lightNDCPosition = output.position;
    
    return output;
}

// Vertex Shader for outline pass (expanded geometry)
VS_OUTPUT VSOutline(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // Expand vertices along normal for outline
    float3 expandedPosition = input.position + input.normal * outlineThickness;
    
    float4 worldPos = mul(float4(expandedPosition, 1.0f), world);
    output.worldPosition = worldPos.xyz;
    output.position = mul(float4(expandedPosition, 1.0f), wvp);
    output.worldNormal = mul(input.normal, (float3x3) world);
    output.worldTangent = mul(input.tangent, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - worldPos.xyz);
    output.lightNDCPosition = output.position;
    
    return output;
}

float GetOutlineFactor(float3 viewDir, float3 normal, float width)
{
    float edgeFactor = dot(viewDir, normal);
    return smoothstep(width - 0.01, width + 0.01, edgeFactor);
}

float4 QuantizeColor(float4 color, float levels)
{
    return floor(color * levels) / levels;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    if (useNormalMap)
    {
        float3 t = normalize(input.worldTangent);
        float3 b = normalize(cross(n, t));
        float3x3 tbn = float3x3(t, b, n);
        float4 normalMapColor = normalMap.Sample(textureSampler, input.texCoord);
        float3 unpackedNormal = normalize(float3((normalMapColor.xy * 2.0f) - 1.0f, normalMapColor.z));
        n = normalize(mul(unpackedNormal, tbn));
    }

    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    float shadowFactor = 1.0f;
    if (useShadowMap)
    {
        float actualDepth = 1.0f - (input.lightNDCPosition.z / input.lightNDCPosition.w);
        float2 shadowUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
        float u = (shadowUV.x + 1.0f) * 0.5f;
        float v = 1.0f - (shadowUV.y + 1.0f) * 0.5f;
        
        if (saturate(u) == u && saturate(v) == v)
        {
            float savedDepth = shadowMap.Sample(textureSampler, float2(u, v)).r;
            shadowFactor = (savedDepth > actualDepth + 0.000003f) ? 0.5f : 1.0f;
        }
    }

    // Toon-style diffuse lighting (stepped/banded)
    float diffuseFactor = saturate(dot(n, light));
    diffuseFactor = floor(diffuseFactor * colorBands) / colorBands;
    diffuseFactor = max(0.2, diffuseFactor) * shadowFactor;

    // Binary specular highlight
    float3 r = reflect(-light, n);
    float specularFactor = pow(saturate(dot(r, view)), materialShininess);
    specularFactor = step(specularThreshold, specularFactor);

    // Texture sampling
    float4 diffuseColor = useDiffuseMap ? diffuseMap.Sample(textureSampler, input.texCoord) : float4(1, 1, 1, 1);

    // Combine lighting
    float4 finalColor = (materialEmissive + lightAmbient * materialAmbient + lightDiffuse * materialDiffuse * diffuseFactor) * diffuseColor;
    finalColor += specularFactor * lightSpecular * materialSpecular;

    // Stylized post-processing
    finalColor = QuantizeColor(finalColor, colorBands);
    finalColor = pow(finalColor, 1.2);

    return finalColor;
}

float4 PSOutline(VS_OUTPUT input) : SV_Target
{
    return float4(0, 0, 0, 1);
}