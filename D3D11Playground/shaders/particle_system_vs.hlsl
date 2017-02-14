struct VSInput
{
	float4 position: POSITION;
};

struct VSOutput
{
	float4 position: POSITION;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position = input.position;
	return output;
}