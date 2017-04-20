Texture2D InputTex            : register(t0);
RWTexture2D<float4> OutputTex : register(u0);

#define N 256
#define CacheSize (N + 2*5)
groupshared float4 Cache[CacheSize];

[numthreads(N, 1, 1)]
void CS_HorizontalBlurPass(int3 groupThreadID : SV_GroupThreadID, int3 dispatchThreadID : SV_DispatchThreadID)
{

	float Weights[11] =
	{
		0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
	};

	int Radius_blur = 5;

	
	if (groupThreadID.x < Radius_blur)
	{
		int x = max(dispatchThreadID.x - Radius_blur, 0);
		Cache[groupThreadID.x] = InputTex[int2(x, dispatchThreadID.y)];
	}

	if (groupThreadID.x >= N - Radius_blur)
	{
		int x = min(dispatchThreadID.x + Radius_blur, InputTex.Length.x - 1);
		Cache[groupThreadID.x + 2 * Radius_blur] = InputTex[int2(x, dispatchThreadID.y)];
	}

	
	Cache[groupThreadID.x + Radius_blur] = InputTex[min(dispatchThreadID.xy, InputTex.Length.xy - 1)];




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
			int k = groupThreadID.x + Radius_blur + i;

			blur_Color += Weights[i + Radius_blur] * Cache[k];
		}

		OutputTex[dispatchThreadID.xy] = blur_Color;
	}
	else
	{
		OutputTex[dispatchThreadID.xy] = InputTex[dispatchThreadID.xy];
	}

	
	
}




