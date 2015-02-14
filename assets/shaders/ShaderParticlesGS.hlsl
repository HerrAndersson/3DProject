struct GS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

[maxvertexcount(3)]
void main(triangle float4 input[3] : SV_POSITION, inout TriangleStream< GS_OUT > output)
{
	for (uint i = 0; i < 3; i++)
	{
		GS_OUT element;
		element.pos = input[i];
		element.tex = float2(1, 1);
		element.normal = float3(0, 0, 0);
		output.Append(element);
	}
}