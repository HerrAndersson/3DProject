Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D shadowTexture : register(t2);
Texture2D worldPosTexture : register(t3);

SamplerState SampleTypePoint : register(s0);

SamplerState sampAni
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressU = WRAP;
};

SamplerState sampPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

cbuffer LightBuffer : register(b0)
{
	matrix lightVP;
	float3 lightDirection;
	int size;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_TARGET
{
	float4 diffuse = float4(0.2f, 0.6f, 0.5f, 1.0f);
	//float4 ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 pos = worldPosTexture.Sample(sampPoint, input.tex);
	float4 col = colorTexture.Sample(sampPoint, input.tex);
	float4 normal = normalTexture.Sample(sampPoint, input.tex);


	float4 p = float4(pos.xyz, 1.0f);
	float4 shadowPos = mul(p, lightVP);
	shadowPos.xy /= shadowPos.w;

	float2 smTex = float2(0.5f* shadowPos.x + 0.5f, -0.5f * shadowPos.y + 0.5f);

	float depth = shadowPos.z / shadowPos.w;
	//depth = 1 - shadowPos.z / shadowPos.w;

	//PCF här
	//float depthSample = shadowTexture.Sample(sampPoint, smTex).r;


	// 2x2 percentage closest filter. POINT SAMPLING!
	float dx = 1.0f / size; // size of one texture sample in the shadow map (width==height)
	float s0 = (shadowTexture.Sample(sampPoint, smTex).r + 0.00005f < depth) ? 0.0f : 1.0f;
	float s1 = (shadowTexture.Sample(sampPoint, smTex + float2(dx, 0.0f)).r + 0.00005f < depth) ? 0.0f : 1.0f;
	float s2 = (shadowTexture.Sample(sampPoint, smTex + float2(0.0f, dx)).r + 0.00005f < depth) ? 0.0f : 1.0f;
	float s3 = (shadowTexture.Sample(sampPoint, smTex + float2(dx, dx)).r + 0.00005f < depth) ? 0.0f : 1.0f;

	// Transform shadow map UV coord to texel space
	float2 texelPos = smTex * size;

	// Determine the lerp amount, if  = (340.3, 200.1) then  = (0.3,0.1)
	float2 lerps = frac(texelPos);
	float shadowCoeff = lerp(lerp(s0, s1, lerps.x), lerp(s2, s3, lerps.x), lerps.y);
	float4 litColor = col * shadowCoeff;

	return litColor;


	//if (depthSample >= depth)
	//{
	//	col = saturate(col * depthSample);
	//}

	//Get local illumination from the "sun" on the whole scene
	float3 lightDir = -lightDirection;																    // Invert the light direction for calculations.
	
	float lightIntensity = saturate(dot(normal.xyz, lightDir)) + 0.35;									// Calculate the amount of light on this pixel.
	float4 outputColor = saturate(litColor * lightIntensity);											    // Determine the final amount of diffuse color based on the color of the pixel combined with the light intensity.

	return outputColor;








	// 2x2 percentage closest filter. POINT SAMPLING!
	//float dx = 1.0f / size; // size of one texture sample in the shadow map (width==height)
	//float s0 = (shadowTexture.Sample(SampleTypePoint, smTex).r + 0.00005f < depth) ? 0.0f : 1.0f;
	//float s1 = (shadowTexture.Sample(SampleTypePoint, smTex + float2(dx, 0.0f)).r + 0.00005f < depth) ? 0.0f : 1.0f;
	//float s2 = (shadowTexture.Sample(SampleTypePoint, smTex + float2(0.0f, dx)).r + 0.00005f < depth) ? 0.0f : 1.0f;
	//float s3 = (shadowTexture.Sample(SampleTypePoint, smTex + float2(dx, dx)).r + 0.00005f < depth) ? 0.0f : 1.0f;

	//// Transform shadow map UV coord to texel space
	//float2 texelPos = smTex * size;

	//// Determine the lerp amount, if  = (340.3, 200.1) then  = (0.3,0.1)
	//float2 lerps = frac(texelPos);
	//float shadowCoeff = lerp(lerp(s0, s1, lerps.x), lerp(s2, s3, lerps.x), lerps.y);
	//float4 litColor = texColor * shadow;
}