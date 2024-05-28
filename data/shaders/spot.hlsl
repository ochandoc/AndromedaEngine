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
  float3 CameraPos;
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
  
  float3 ViewDir = normalize(CameraPos - input.WorldPos);
  float3 LightDir = normalize(Position - input.WorldPos);
  float CutOff = cos(InnerConeAngle * 3.1415f / 180.0f);
  float OuterCutOff = cos(OuterConeAngle * 3.1415f / 180.0f);
  
  float Distance = length(Position - input.WorldPos);
  float AttenuationAmount = 1.0f / ((ConstantAttenuation + LinearAttenuation * Distance) + (QuadraticAttenuation * Distance * Distance));
  
  float Theta = dot(LightDir, normalize(-Direction));
  float Epsilon = (CutOff - OuterCutOff);
  float Intensity = clamp((Theta - OuterCutOff) / Epsilon, 0.0f, 1.0f);
  
  float diff = max(dot(input.Normal, LightDir), 0.0f);
  float3 diffuse = diff * DiffuseColor * AttenuationAmount * Intensity;
  
  float3 ReflectDir = reflect(-LightDir, input.Normal);
  float spec = pow(max(dot(ViewDir, ReflectDir), 0.0f), SpecularShininess);
  float3 specular = SpecularStrength * spec * float3(1.0f, 1.0f, 1.0f) * AttenuationAmount * Intensity;
  
  float3 LightColor = diffuse + specular;
  
  output.Color = float4(LightColor * object_color.xyz, object_color.w);
  return output;
}
