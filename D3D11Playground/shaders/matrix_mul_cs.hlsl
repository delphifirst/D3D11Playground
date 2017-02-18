cbuffer Parameters : register(b0)
{
	int kSize;
	int3 unused;
}

StructuredBuffer<float> input : register(t0);

RWStructuredBuffer<float> result : register(u0);

[numthreads(256, 1, 1)]
void main(int3 dispatch_thread_id: SV_DispatchThreadID)
{
	int i = dispatch_thread_id.x / kSize;
	int j = dispatch_thread_id.x % kSize;

	float temp = 0;
	for (int k = 0; k < kSize; ++k)
		temp += input[i * kSize + k] * input[k * kSize + j];
	result[dispatch_thread_id.x] = temp;
}