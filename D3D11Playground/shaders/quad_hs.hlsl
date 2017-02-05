struct PatchTess
{
	float edge_tess[4] : SV_TessFactor;
	float inside_tess[2] : SV_InsideTessFactor;
};

struct VSOutput
{
	float4 position: POSITION;
	float2 tex_coord: TEXCOORD;
};

PatchTess constant_hs(InputPatch<VSOutput, 4> patch, uint patch_id: SV_PrimitiveID)
{
	PatchTess result;
	result.edge_tess[0] = result.edge_tess[1]
		= result.edge_tess[2] = result.edge_tess[3]
		= result.inside_tess[0] = result.inside_tess[1] = 64;

	return result;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("constant_hs")]
[maxtessfactor(64.0f)]
VSOutput main(InputPatch<VSOutput, 4> patch, uint point_id: SV_OutputControlPointID,
	uint patch_id : SV_PrimitiveID)
{
	return patch[point_id];
}