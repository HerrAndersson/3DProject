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
	float4 worldPos : POS;
};

struct PS_OUT
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 pos : SV_Target2;
};

PS_OUT main(VS_OUT input) : SV_Target
{
	PS_OUT output = (PS_OUT)0;
	output.color = txDiffuse.Sample(sampAni, input.tex);
	output.normal = float4(input.normal, 0);
	output.pos = input.worldPos;

	return output;
}