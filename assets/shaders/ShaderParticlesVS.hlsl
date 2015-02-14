cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix wvpMatrix;
};

struct VS_IN
{
	float4 position : POSITION;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	input.position.w = 1.0f;
	output.position = mul(input.position, wvpMatrix);

	return output;
}