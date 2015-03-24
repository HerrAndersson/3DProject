cbuffer LightBuffer
{
	matrix lightWVP;
	float3 lightPos;
};

float4 main(float3 posL : POSITION) : SV_POSITION
{
	// Render from light's perspective.
	float4 pos = float4(posL, 1.0f);
	pos = mul(pos, lightWVP);
	return pos;
}