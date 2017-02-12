cbuffer PerObject : register(b0)
{
	float4x4 view_matrix;
	float4x4 proj_matrix;
};

struct VSInput
{
	float4 position: POSITION;
	float2 tex_coord: TEXCOORD;
};

struct VSOutput
{
	float4 position: SV_POSITION;
	float2 tex_coord: TEXCOORD;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position = mul(input.position, view_matrix);
	output.position = mul(output.position, proj_matrix);
	output.tex_coord = input.tex_coord;
	return output;
}