SamplerState SampleType;
Texture2D shaderTexture[4];

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
	float4 blendMapColor = shaderTexture[0].Sample(SampleType, input.Tex);
	float4 textureColor;
	int repeat = 32;
	textureColor =  shaderTexture[1].Sample(SampleType, input.Tex*repeat) * blendMapColor.r;
	textureColor += shaderTexture[2].Sample(SampleType, input.Tex*repeat) * blendMapColor.g;
	textureColor += shaderTexture[3].Sample(SampleType, input.Tex*repeat) * blendMapColor.b;

	float4 color = ambientColor;
	float3 lightDir = -lightDirection;
	float lightIntensity = saturate(dot(input.Normal, lightDir));
	
	if (lightIntensity > 0.0f)
	{
		color += (diffuseColor * lightIntensity);
	}
	
	color = saturate(color);
	color = color * textureColor;

	return color;
}