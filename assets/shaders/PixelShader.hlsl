struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(VS_OUT input) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}