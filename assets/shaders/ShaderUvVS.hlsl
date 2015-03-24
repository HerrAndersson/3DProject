cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VS_IN
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPos : POS;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	input.position.w = 1.0f;

	//output.worldPos = float4(input.position.xyz, 1.0f);
	output.worldPos = mul(float4(input.position.xyz, 1.0f), worldMatrix);
	//output.worldPos = mul(output.worldPos, worldMatrix);

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}