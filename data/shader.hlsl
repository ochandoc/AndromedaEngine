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

VS_OUTPUT main(VS_INPUT input)
{
  VS_OUTPUT output;
  output.Pos = float4(input.Pos, 1.0f);
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

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
  PS_OUTPUT output;
  output.Color = float4(input.UV, 0.0f, 1.0f);
  return output;
}