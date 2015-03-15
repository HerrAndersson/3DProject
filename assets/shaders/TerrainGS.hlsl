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

	//Calculate the vector from the camera to the triangle
	float3 camDir = normalize(input[0].pos - camPos).xyz;

	//If the dot is positive it means that the triangle is facing away from the camera
	if (dot(n, camDir) < 0.05f)
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