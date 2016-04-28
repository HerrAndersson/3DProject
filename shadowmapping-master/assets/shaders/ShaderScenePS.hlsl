Texture2D txDiffuse : register(t0);
Texture2D shadowMap : register(t1);

SamplerComparisonState shadowSampler : register(s0);
SamplerState sampPoint : register(s1);

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightSpacePos : POSITION1;
};

float4 main(VS_OUT input) : SV_Target
{
	const float3 ambient = float3(0.1f, 0.1f, 0.1f);

	float2 smTex;
	smTex.x = 0.5f + (input.lightSpacePos.x / input.lightSpacePos.w * 0.5f);
	smTex.y = 0.5f - (input.lightSpacePos.y / input.lightSpacePos.w * 0.5f);

	float depth = input.lightSpacePos.z / input.lightSpacePos.w;

	float epsilon = 0.001;
	epsilon = clamp(epsilon, 0, 0.1);

	float dx = 1.0f / 2048;
	float s0 = float(shadowMap.SampleCmpLevelZero(shadowSampler, smTex, depth + epsilon));
	float s1 = float(shadowMap.SampleCmpLevelZero(shadowSampler, smTex + float2(dx, 0.0f), depth + epsilon));
	float s2 = float(shadowMap.SampleCmpLevelZero(shadowSampler, smTex + float2(0.0f, dx), depth + epsilon));
	float s3 = float(shadowMap.SampleCmpLevelZero(shadowSampler, smTex + float2(dx, dx), depth + epsilon));

	float2 texelPos = smTex * 2048;

	float2 lerps = frac(texelPos);
	float shadowCoeff = lerp(lerp(s0, s1, lerps.x), lerp(s2, s3, lerps.x), lerps.y);

	float3 lightDir = float3(0.0f, -0.6f, 0.75f) * -1;
	float lightIntensity = saturate(dot(input.normal.xyz, lightDir)) + 0.15;
	float4 outputColor = saturate(txDiffuse.Sample(sampPoint, input.tex)*lightIntensity);

	if (shadowCoeff < depth)
	{
		return outputColor * shadowCoeff;
	}

	return outputColor;
}