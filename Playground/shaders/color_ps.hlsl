cbuffer Global
{
	float4 color;
};

struct PSInput
{
	float4 position: SV_POSITION;
};

struct PSOutput
{
	float4 color: SV_TARGET;
};

PSOutput main(PSInput input)
{
	PSOutput output;
	output.color = color;
	return output;
}