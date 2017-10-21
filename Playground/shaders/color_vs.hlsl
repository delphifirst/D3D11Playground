cbuffer Global
{
	float4x4 view_proj;
	float4x4 world;
}

struct VSInput
{
	float3 position: POSITION;
};

struct VSOutput
{
	float4 position: SV_POSITION;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position = mul(view_proj, mul(world, float4(input.position, 1)));
	return output;
}