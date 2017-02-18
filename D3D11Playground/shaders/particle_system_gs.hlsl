cbuffer PerObject : register(b0)
{
	float4 x;
	float4 y;
	float4 size;
	float4x4 view_matrix;
	float4x4 proj_matrix;
};

struct GSInput
{
	float4 position: POSITION;
	float4 speed: SPEED;
	float2 life_time_info: LIFE_TIME_INFO;
};

struct GSOutput
{
	float4 position: SV_POSITION;
	float2 tex_coord: TEXCOORD;
};

[maxvertexcount(3)]
void main(point GSInput input[1], inout TriangleStream<GSOutput> output_stream)
{
	GSOutput output;

	output.position = input[0].position + (-size.x / 2) * x + (size.y / 2) * y;
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, proj_matrix);
	output.tex_coord = float2(0, 0);
	output_stream.Append(output);

	output.position = input[0].position + (3 * size.x / 2) * x + (size.y / 2) * y;
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, proj_matrix);
	output.tex_coord = float2(2, 0);
	output_stream.Append(output);

	output.position = input[0].position + (-size.x / 2) * x + (- 3 * size.y / 2) * y;
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, proj_matrix);
	output.tex_coord = float2(0, 2);
	output_stream.Append(output);
}