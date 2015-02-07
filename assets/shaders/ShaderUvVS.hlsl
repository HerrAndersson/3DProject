cbuffer VS_CONSTANT_BUFFER
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float2 Uvcoord : UVCOORD;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Uvcoord : UVCOORD;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;


	float4 temp = float4(input.Pos, 1);

	output.Pos = mul(temp, worldMatrix);
	output.Pos = mul(output.Pos, viewMatrix);
	output.Pos = mul(output.Pos, projectionMatrix);


	output.Uvcoord = input.Uvcoord;
	return output;
}