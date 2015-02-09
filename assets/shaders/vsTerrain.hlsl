
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
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, wvpMatrix);
	//output.position = mul(input.position, worldMatrix);
	//output.position = mul(output.position, viewMatrix);
	//output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	return output;
}

//struct VS_IN
//{
//	float3 Pos : POSITION;
//	float3 Color : COLOR;
//};
//
//struct VS_OUT
//{
//	float4 Pos : SV_POSITION;
//	float3 Color : COLOR;
//};

//VS_OUT main(VS_IN input)
//{
//	VS_OUT output = (VS_OUT)0;
//
//	output.Pos = float4(input.Pos, 1);
//
//	//output.Pos = mul(input.Pos, wvpMatrix);
//
//	//output.Pos = mul(input.Pos, worldMatrix);
//	output.Pos = mul(output.Pos, viewMatrix);
//	output.Pos = mul(output.Pos, projectionMatrix);
//
//	output.Color = input.Color;
//
//	return output;
//}



//PS
//Texture2D shaderTexture;
//SamplerState SampleType;
//
//cbuffer LightBuffer
//{
//	float4 ambientColor;
//	float4 diffuseColor;
//	float3 lightDirection;
//	float padding;
//};

//struct VS_OUT
//{
//	float4 position : SV_POSITION;
//	float2 tex : TEXCOORD0;
//	float3 normal : NORMAL;
//};
//
//float4 main(VS_OUT input) : SV_Target
//{	
//	return input.position;
//}