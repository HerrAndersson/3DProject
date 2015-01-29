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
	float4 Pos : SV_POSITION;
	float2 Uvcoord : UVCOORD;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 s = txDiffuse.Sample(sampAni, input.Uvcoord).xyz;
	return float4(s, 1.0f);
}