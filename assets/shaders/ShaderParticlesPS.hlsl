Texture2D txDiffuse : register(t0);
SamplerState sampAni
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressU = WRAP;
};

struct GS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PS_OUT
{
	float4 color : SV_Target0;
	float3 normal : SV_Target1;
};


PS_OUT main(GS_OUT input) : SV_Target
{
	PS_OUT output = (PS_OUT)0;

	float3 s = txDiffuse.Sample(sampAni, input.tex).xyz;

	output.color = float4(s, 1.0f);
	output.normal = input.normal;

	return output;
}