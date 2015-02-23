Texture2D shaderTexture : register(t0);
SamplerState SampleTypeWrap : register(s0);

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PS_OUT
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
};

PS_OUT main(VS_OUT input) : SV_TARGET
{
	PS_OUT output = (PS_OUT)0;

	// Sample the color from the texture and store it for output to the render target.
	output.color = shaderTexture.Sample(SampleTypeWrap, input.tex);

	// Store the normal for output to the render target.
	output.normal = float4(input.normal, 1.0f);

	return output;
}