cbuffer PerObject : register(b0)
{
	float4x4 world_matrix;
	float4x4 view_matrix;
	float4x4 proj_matrix;
};

struct PatchTess
{
	float edge_tess[4] : SV_TessFactor;
	float inside_tess[2] : SV_InsideTessFactor;
};

struct HSOutput
{
	float4 position: POSITION;
	float2 tex_coord: TEXCOORD;
};

struct DSOutput
{
	float4 position: SV_POSITION;
	float2 tex_coord: TEXCOORD;
};

[domain("quad")]
DSOutput main(PatchTess patch_tess, float2 uv: SV_DomainLocation,
	const OutputPatch<HSOutput, 4> quad)
{
	HSOutput output;

	float3 p1 = lerp(quad[0].position, quad[1].position, uv.x);
	float3 p2 = lerp(quad[2].position, quad[3].position, uv.x);
	output.position = float4(lerp(p1, p2, uv.y), 1);
	output.position.y += sin(uv.x * 20);

	output.position = mul(output.position, world_matrix);
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, proj_matrix);
	output.tex_coord = uv;

	return output;
}