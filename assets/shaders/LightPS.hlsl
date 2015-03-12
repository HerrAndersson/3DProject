Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState SampleTypePoint : register(s0);

cbuffer LightBuffer
{
	float3 lightDirection;
	float padding;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_TARGET
{

	float4 colors = colorTexture.Sample(SampleTypePoint, input.tex);		  // Sample the colors from the color render texture using the point sampler at this texture coordinate location.
	float4 normals = normalTexture.Sample(SampleTypePoint, input.tex);		  // Sample the normals from the normal render texture using the point sampler at this texture coordinate location.

	float3 lightDir = -lightDirection;										  // Invert the light direction for calculations.

	float lightIntensity = saturate(dot(normals.xyz, lightDir));			  // Calculate the amount of light on this pixel.
	float4 outputColor = saturate(colors * lightIntensity);				      // Determine the final amount of diffuse color based on the color of the pixel combined with the light intensity.

	return outputColor;
}