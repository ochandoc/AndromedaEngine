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
  float4x4 vpm = mul(view, projection);
  
  float3 CameraRight = float3(view[0][0], view[1][0], view[2][0]);
  float3 CameraUp = float3(view[0][1], view[1][1], view[2][1]);
  float3 center = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), model).xyz;
  
  float3 VertexWorldPos = center + CameraRight * input.Pos.x + CameraUp * input.Pos.y;
  
  VS_OUTPUT output;
  output.Pos = mul(float4(VertexWorldPos, 1.0f), vpm);
  output.UV = input.UV;
  return output;
}

#type Pixel
struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float2 UV : TEXCOORD;
};

struct PS_OUTPUT
{
  float4 Color : SV_Target0;
};

cbuffer ObjectData : register(b0)
{
  float4 Color;
  float3 CameraPos;
  bool HasColorTexture;
};

Texture2D ColorTexture : COLOR_TEXTURE : register(t0);
SamplerState ColorSampler : COLOR_SAMPLER : register(s0);

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
  PS_OUTPUT output;
  
  if (HasColorTexture)
    output.Color = ColorTexture.Sample(ColorSampler, input.UV);
  else
    output.Color = Color;
  
  return output;
}
