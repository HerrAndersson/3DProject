SamplerState SampleType;
Texture2D shaderTexture[4];

cbuffer LightBuffer : register(cb0)
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct PS_OUT
{
	float4 color : SV_Target0;
	float3 normal : SV_Target1;
};

PS_OUT main(VS_OUT input) : SV_Target
{
	PS_OUT output = (PS_OUT)0;

	float4 blendMapColor = shaderTexture[0].Sample(SampleType, input.Tex);
	float4 textureColor;
	int repeat = 32;
	textureColor =  shaderTexture[1].Sample(SampleType, input.Tex*repeat) * blendMapColor.r;
	textureColor += shaderTexture[2].Sample(SampleType, input.Tex*repeat) * blendMapColor.g;
	textureColor += shaderTexture[3].Sample(SampleType, input.Tex*repeat) * blendMapColor.b;

	output.color = textureColor;
	output.normal = input.Normal;

	return output;
}