
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix wvpMatrix;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.Pos = float4(input.Pos, 1);

	//output.Pos = mul(input.Pos, wvpMatrix);

	//output.Pos = mul(input.Pos, worldMatrix);
	output.Pos = mul(output.Pos, viewMatrix);
	output.Pos = mul(output.Pos, projectionMatrix);

	//Calculate the normal vector against the world matrix only.
	output.Normal = mul(input.Normal, (float3x3)worldMatrix);

	//Normalize the normal vector.
	output.Normal = normalize(output.Normal);

	return output;
}