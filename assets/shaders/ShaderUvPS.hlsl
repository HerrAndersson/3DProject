Texture2D txDiffuse : register(t0);
SamplerState sampAni
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressU = WRAP;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 s = txDiffuse.Sample(sampAni, input.tex).xyz;
	return float4(s, 1.0f);
}