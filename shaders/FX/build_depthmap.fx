cbuffer cbPerFrame
{

	float4x4 LightWVP;

};


Texture2D ObjectTexture;

SamplerState samTrilinearAddrUVWrap
{

	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;

};

struct VS_IN
{

	float3 lPos:  POSITION;
	float3 lNorm: NORMAL;
	float3 vel:   VELOCITY;
	float2 TexC:  TEXCOORD;

};


struct VS_OUT
{

	float4 PosH : SV_POSITION;
	float2 TexC : TEXCOORD;
	float2 depth: DEPTH;

};


VS_OUT VS(VS_IN vIn)
{

	VS_OUT vOut;

	
		vOut.PosH = mul(float4(vIn.lPos, 1.0f), LightWVP);
	//vOut.PosH = float4(vIn.lPos, 1.0f);
	//vOut.PosH = float4(0,0,0,1);

		vOut.depth.xy = vOut.PosH.zw;


		vOut.TexC = vIn.TexC;


	return vOut;

}


//void PS(VS_OUT v)
float4 PS(VS_OUT v): SV_TARGET
{

	return v.depth.x/v.depth.y;



/*
	float4 diffuse = ObjectTexture.Sample(samTrilinearAddrUVWrap, v.TexC);

		// Shadows can be cast only by visible pixels;
		// clip discards the current pixel from further processing if x < 0 - therefore if pixel is transparent it'll be discarded

	clip(diffuse.a - 0.15f);
*/

}

technique11 DepthMapCreateTech
{

	pass P0
	{
	
		SetVertexShader( CompileShader(vs_4_0, VS() ) );
			SetGeometryShader(NULL);
		SetPixelShader( CompileShader(ps_4_0, PS() ) );

	}


}