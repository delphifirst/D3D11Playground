cbuffer GlobalVS : register(b0)
{
	float4x4 view_proj;
}

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
	output.position = float4(input.position, 1);
	output.color = input.color;
	return output;
}