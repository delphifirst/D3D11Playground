Texture2D shader_texture : register(t0);

SamplerState linear_sampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

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
	output.color = shader_texture.Sample(linear_sampler, input.tex_coord);
	return output;
}