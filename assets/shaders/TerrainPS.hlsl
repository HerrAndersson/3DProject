SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
};

float4 main(VS_OUT input) : SV_Target
{
	//Set default to ambient light
	float4 color = ambientColor;
	//Invert the light direction for calculations.
	float3 lightDir = -lightDirection;
	//Calculate the amount of light on this pixel.
	float lightIntensity = saturate(dot(input.Normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		color += (diffuseColor * lightIntensity);
	}

	color = saturate(color);

	return color;
}