#type Vertex

struct VS_INPUT
{
  float3 Pos : POSITION;
  float3 Normal : NORMAL;
  float2 UV : TEXCOORD;
};

struct VS_OUTPUT
{
  float4 Pos : SV_POSITION;
  float3 WorldPos : WORLD_POSITION;
  float3 Normal : NORMAL;
  float2 UV : TEXCOORD;
};

cbuffer Object : register(b0)
{
  float4x4 view;
  float4x4 projection;
  float4x4 model;
};

VS_OUTPUT main(VS_INPUT input)
{
  float4x4 vpm = mul(model, mul(view, projection));

  VS_OUTPUT output;
  output.Pos = mul(float4(input.Pos, 1.0f), vpm);
  output.WorldPos = mul(float4(input.Pos, 1.0f), model).xyz;
  output.Normal = mul(float4(input.Normal, 0.0f), model).xyz;
  output.UV = input.UV;
  return output;
}

#type Pixel
struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float3 WorldPos : WORLD_POSITION;
  float3 Normal : NORMAL;
  float2 UV : TEXCOORD;
};

struct PS_OUTPUT
{
  float4 Color : SV_Target0;
};

cbuffer ObjectData : register(b0)
{
  float4 Color;
  bool HasColorTexture;
};

cbuffer LightData : register(b1)
{
  float3 Position;
  float ConstantAttenuation;
  float3 Direction;
  float LinearAttenuation;
  float3 DiffuseColor;
  float QuadraticAttenuation;
  float SpecularStrength;
  float SpecularShininess;
  float InnerConeAngle;
  float OuterConeAngle;
};

Texture2D ColorTexture : COLOR_TEXTURE : register(t0);
SamplerState ColorSampler : COLOR_SAMPLER : register(s0);

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
  PS_OUTPUT output;
  
  float4 object_color;
  if (HasColorTexture)
    object_color = ColorTexture.Sample(ColorSampler, input.UV);
  else
    object_color = Color;
  
  float diff = max(dot(input.Normal, Direction), 0.0f);
  float3 diffuse = diff * DiffuseColor * object_color.xyz;

  output.Color = float4(diffuse, 1.0f);
  return output;
}
