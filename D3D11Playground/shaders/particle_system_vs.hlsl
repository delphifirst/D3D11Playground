struct Particle
{
	float3 position: POSITION;
	float3 speed: SPEED;
	float2 life_time_info: LIFE_TIME_INFO;
};

Particle main(Particle input)
{
	return input;
}