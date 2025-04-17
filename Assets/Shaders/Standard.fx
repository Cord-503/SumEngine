// Description: standard effect applies to lighting

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    matrix world;
    float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialEmissive;
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;

    float materialShininess;
}

Texture2D diffuseMap : register(t0);

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
    float2 texCoord : TEXCOORD;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldNormal = mul(input.normal, (float3x3) world);
    output.worldTangent = mul(input.tangent, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - (mul(float4(input.position, 1.0f), world).xyz));
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    // emissive
    float4 emissive = materialEmissive;
    
    // ambient
    float4 ambient = lightAmbient * materialAmbient;
    
    // diffuse
    float4 d = saturate(dot(n, light));
    float4 diffuse = d * lightDiffuse * materialDiffuse;
    
    // specular
    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, materialShininess);
    float4 specular = s * lightSpecular * materialSpecular;
    
    // texture color
    float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);
    
    // final color
    float4 finalColor = (emissive + ambient + diffuse) * diffuseColor + specular;
    
    return finalColor;
}