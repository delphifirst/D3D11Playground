cbuffer PerObject : register(b1)
{
	float3 emit_position;
	float min_speed;
	float max_speed;
	float min_life_time;
	float max_life_time;
	float delta_time;
};

struct Particle
{
	float3 position: POSITION;
	float3 speed: SPEED;
	float2 life_time_info: LIFE_TIME_INFO;
};

[maxvertexcount(1)]
void main(point Particle input[1], inout PointStream<Particle> output_stream)
{
	Particle output;

	float life_time = input[0].life_time_info.x;
	float current_life_time = input[0].life_time_info.y;

	if (current_life_time < life_time)
	{
		output.life_time_info.x = life_time;
		output.life_time_info.y = current_life_time + delta_time;
		output.speed = input[0].speed;
		output.position = input[0].position + delta_time * output.speed;
	}
	else
	{
		output.life_time_info.x = life_time;
		output.life_time_info.y = 0;
		output.speed = input[0].speed;
		output.position = emit_position;
	}
	output_stream.Append(output);
}