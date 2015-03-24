SamplerState SampleType;
Texture2D shaderTexture[4];

//cbuffer LightBuffer : register(cb0)
//{
//	float4 ambientColor;
//	float4 diffuseColor;
//	float3 lightDirection;
//	float padding;
//};

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPos : POS;
};

struct PS_OUT
{
	float4 color : SV_Target0;
	float3 normal : SV_Target1;
	float4 worldPos : SV_Target2;
};

PS_OUT main(GS_OUT input) : SV_Target
{
	PS_OUT output = (PS_OUT)0;

	float4 blendMapColor = shaderTexture[0].Sample(SampleType, input.tex);
	float4 textureColor;
	int repeat = 32;
	textureColor =  shaderTexture[1].Sample(SampleType, input.tex*repeat) * blendMapColor.r;
	textureColor += shaderTexture[2].Sample(SampleType, input.tex*repeat) * blendMapColor.g;
	textureColor += shaderTexture[3].Sample(SampleType, input.tex*repeat) * blendMapColor.b;

	output.color = textureColor;
	output.normal = input.normal;
	output.worldPos = input.worldPos;

	return output;
}