cbuffer gsBuffer
{
	float3 camPos;
	matrix viewMatrix;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

[maxvertexcount(3)]
void main(triangle VS_OUT input[3], inout TriangleStream<GS_OUT> OutputStream)
{
	GS_OUT output = (GS_OUT)0;

	//Calculate the face normal
	float3 A = (input[1].pos - input[0].pos);
	float3 B = (input[2].pos - input[0].pos);
	float3 n = normalize(cross(A, B));

	float3 camDir = normalize(input[0].pos - camPos).xyz;

	if (dot(camDir, n) < 0.0f)
	{
		for (int i = 0; i < 3; i++)
		{
			output.pos = input[i].pos;
			output.tex = input[i].tex;
			output.normal = input[i].normal.xyz;

			OutputStream.Append(output);
		}
	}
}