cbuffer Parameters : register(b0)
{
	uint kSize;
	int3 unused;
}

StructuredBuffer<float> input : register(t0);

RWStructuredBuffer<float> result : register(u0);

[numthreads(256, 1, 1)]
void main(int3 dispatch_thread_id: SV_DispatchThreadID)
{
	uint i = uint(dispatch_thread_id.x) / kSize;
	uint j = uint(dispatch_thread_id.x) % kSize;

	float temp = 0;
	for (uint k = 0; k < kSize; ++k)
		temp += input[i * kSize + k] * input[k * kSize + j];
	result[dispatch_thread_id.x] = temp;
}