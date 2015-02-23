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
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	return output;
}