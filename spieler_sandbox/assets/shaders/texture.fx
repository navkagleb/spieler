#include "light.fx"

struct PerPass
{
    float4x4 View;
    float4x4 Projection;
    float3 W_CameraPosition;
    float4 AmbientLight;
    Light::LightContainer Lights;
};

struct PerObject
{
    float4x4 World;
};

ConstantBuffer<PerPass> g_Pass : register(b0);
ConstantBuffer<PerObject> g_Object : register(b1);

struct VS_Input
{
    float3 PositionL : Position;
    float3 NormalL : Normal;
    float3 Tangent : Tangent;
    float2 TexCoord : TexCoord;
};

struct VS_Output
{
    float4 PositionH : SV_Position;
    float3 PositionW : Position;
    float3 NormalW : Normal;
    float2 TexCoord : TexCoord;
};

VS_Output VS_Main(VS_Input input)
{
    VS_Output output = (VS_Output) 0;
    output.PositionW = mul(float4(input.PositionL, 1.0f), g_Object.World).xyz;
    output.NormalW = mul(input.NormalL, (float3x3) g_Object.World);

    float4 position = float4(output.PositionW, 1.0f);
    position = mul(position, g_Pass.View);
    position = mul(position, g_Pass.Projection);

    output.PositionH = position;
    output.TexCoord = input.TexCoord;

    return output;
}

Texture2D g_Texture : register(t0);

SamplerState g_SamplerPointWrap : register(s0);
SamplerState g_SamplerPointClamp : register(s1);
SamplerState g_SamplerLinearWrap : register(s2);
SamplerState g_SamplerLinearClamp : register(s3);
SamplerState g_SamplerAnisotropicWrap : register(s4);
SamplerState g_SamplerAnisotropicClamp : register(s5);

float4 PS_Main(VS_Output input) : SV_Target
{
    float4 g_DiffuseAlbedo = 1.0f; 
    
    float4 diffuseAlbedo = g_Texture.Sample(g_SamplerAnisotropicClamp, input.TexCoord) * g_DiffuseAlbedo;
    
    input.NormalW = normalize(input.NormalW);

    // Vector from point being lit to eye
    const float3 viewVector = normalize(g_Pass.W_CameraPosition - input.PositionW);

    // Indirect lighting
    const float4 ambient = g_Pass.AmbientLight * diffuseAlbedo;

    // Direct lighting
    Light::Material material;
    material.DiffuseAlbedo = 1.0f;
    material.FresnelR0 = 0.05f;
    material.Shininess = 1.0f - 0.9f;

    const float3 shadowFactor = 1.0f;

    float4 litColor = Light::ComputeLighting(g_Pass.Lights, material, input.PositionW, input.NormalW, viewVector, shadowFactor);
    litColor += ambient;

    litColor.a = diffuseAlbedo.a;
    
    return litColor;
}