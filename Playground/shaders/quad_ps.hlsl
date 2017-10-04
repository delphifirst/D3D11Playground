Texture2D shader_texture : register(t0);

SamplerState quad_sampler;

struct PSInput
{
	float4 position: SV_POSITION;
	float2 tex_coord: TEXCOORD;
};

struct PSOutput
{
	float4 color: SV_TARGET;
};

PSOutput main(PSInput input)
{
	PSOutput output;
	output.color = shader_texture.SampleGrad(quad_sampler, input.tex_coord, float2(-1 / 256.0, 0), float2(0, -1 / 256.0));
	return output;
}