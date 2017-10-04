struct VSInput
{
	float3 position: POSITION;
	float3 color: COLOR;
};

struct VSOutput
{
	float4 position: SV_POSITION;
	float3 color: COLOR;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position.xyz = input.position;
	output.position.w = 1;
	output.color = input.color;
	return output;
}