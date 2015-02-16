SamplerState SampleType;
Texture2D shaderTexture;

cbuffer LightBuffer : register(cb0)
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
};

float4 main(VS_OUT input) : SV_Target
{
	float4 textureColor = shaderTexture.Sample(SampleType, input.Tex);		//Sample texture color
	float4 color = ambientColor; 											//Set default to ambient light
	float3 lightDir = -lightDirection;										//Invert the light direction for calculations.
	float lightIntensity = saturate(dot(input.Normal, lightDir)) + 0.0f; 	//Calculate the amount of light on this pixel.

	if (lightIntensity > 0.0f)
	{
		color += (diffuseColor * lightIntensity);
	}

	color = saturate(color);
	color = color * textureColor;

	return color;
}