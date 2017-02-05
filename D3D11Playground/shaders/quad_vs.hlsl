struct VSInput
{
	float4 position: POSITION;
	float2 tex_coord: TEXCOORD;
};

struct VSOutput
{
	float4 position: POSITION;
	float2 tex_coord: TEXCOORD;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position = input.position;
	output.tex_coord = input.tex_coord;
	return output;
}