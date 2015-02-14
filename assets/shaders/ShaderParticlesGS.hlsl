cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix wvpMatrix;
	float3 campos;
};

struct GS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct GS_IN
{
	float4 position : SV_POSITION;
};

[maxvertexcount(6)]
void main(point GS_IN input[1], inout TriangleStream<GS_OUT> OutputStream)
{
	GS_OUT output = (GS_OUT)0;
	float scale = 3;
	//float4 campos = float4(4, 4, 4, 0);
	
	float4 up = float4(0, 1, 0, 0);
	float4 right = float4(normalize(cross(campos, up)).xyz, 0);
	float4 temp;

	up = up*scale;
	right = right*scale;

	//First triangle
	temp = input[0].position + up - right;
	output.position = mul(temp, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = float2(0, 0);
	OutputStream.Append(output);

	temp = input[0].position + up + right;
	output.position = mul(temp, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = float2(1, 0);
	OutputStream.Append(output);

	temp = input[0].position - up + right;
	output.position = mul(temp, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = float2(1, 1);
	OutputStream.Append(output);

	//Second triangle
	temp = input[0].position + up - right;
	output.position = mul(temp, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = float2(0, 0);
	OutputStream.Append(output);

	temp = input[0].position - up - right;
	output.position = mul(temp, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = float2(0, 1);
	OutputStream.Append(output);

	temp = input[0].position - up + right;
	output.position = mul(temp, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = float2(1, 1);
	OutputStream.Append(output);

	OutputStream.RestartStrip();
}