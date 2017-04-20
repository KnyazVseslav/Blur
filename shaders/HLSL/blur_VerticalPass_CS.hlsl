Texture2D InputTex            : register(t0);
RWTexture2D<float4> OutputTex : register(u0);

#define N 256
#define CacheSize (N + 2*5)
groupshared float4 Cache[CacheSize];


[numthreads(1, N, 1)]
void CS_VerticalBlurPass(int3 groupThreadID : SV_GroupThreadID,	int3 dispatchThreadID : SV_DispatchThreadID)
{


	float Weights[11] =
	{
		0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
	};

	int Radius_blur = 5;

	
	if (groupThreadID.y < Radius_blur)
	{
		int y = max(dispatchThreadID.y - Radius_blur, 0);
		Cache[groupThreadID.y] = InputTex[int2(dispatchThreadID.x, y)];
	}
	if (groupThreadID.y >= N - Radius_blur)
	{
		int y = min(dispatchThreadID.y + Radius_blur, InputTex.Length.y - 1);
		Cache[groupThreadID.y + 2 * Radius_blur] = InputTex[int2(dispatchThreadID.x, y)];
	}

	Cache[groupThreadID.y + Radius_blur] = InputTex[min(dispatchThreadID.xy, InputTex.Length.xy - 1)];


	// Synchronize all threads
	GroupMemoryBarrierWithGroupSync();



	//
	// Blur according to the condition
	//
	

	float4 blur_Color = float4(0, 0, 0, 0);
	
	if (dispatchThreadID.x > ceil(InputTex.Length.x/2.0f))
	{

		[unroll]
		for (int i = -Radius_blur; i <= Radius_blur; ++i)
		{
			int k = groupThreadID.y + Radius_blur + i;

			blur_Color += Weights[i + Radius_blur] * Cache[k];
		}

		OutputTex[dispatchThreadID.xy] = blur_Color;
	}
	else
	{
		OutputTex[dispatchThreadID.xy] = InputTex[dispatchThreadID.xy];
	}
	



}

