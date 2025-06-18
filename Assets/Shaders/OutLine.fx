// Outline.fx
cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    matrix world;
    float3 viewPosition;
}

cbuffer SettingsBuffer : register(b1)
{
    float outlineWidth;
    float3 outlineColor;
    float edgeThreshold;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 worldPosition : TEXCOORD0;
    float edgeFactor : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldNormal = normalize(mul(input.normal, (float3x3) world));
    output.worldPosition = mul(float4(input.position, 1.0f), world).xyz;
    
    float3 viewDir = normalize(viewPosition - output.worldPosition);
    output.edgeFactor = 1.0 - abs(dot(output.worldNormal, viewDir));
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float outline = smoothstep(0.1, 0.7, input.edgeFactor) * outlineWidth;
    
    if (outline > 0.001)
    {
        return float4(outlineColor, 1.0);
    }
    
    discard;
    return float4(0, 0, 0, 0);
}