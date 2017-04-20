cbuffer cbPerObject
{
	float4x4 LWVP;
};


struct VS_IN
{
	float3 posL: POSITION;
	float3 norm: NORMAL;
	float3 vel: VELOCITY;
	float2 texC: TEXCOORD;
};

struct VS_OUT
{
	float4 posH: SV_POSITION;
	float3 norm: NORMAL;
	float3 vel: VELOCITY;
	float2 texC: TEXCOORD;
};


Texture2D DiffuseMap;
Texture2D SpecularMap;

Texture2D OverlayMap;


SamplerState sm_Trilinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV= WRAP;
};


VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;

		vOut.posH = mul(float4(vIn.posL, 1.0f), LWVP);
		vOut.norm = vIn.norm;
		vOut.vel = vIn.vel;
		vOut.texC = vIn.texC;

	return vOut;
}


float4 PS(VS_OUT v): SV_TARGET
{
	float4 OutputColor;

			OutputColor= DiffuseMap.Sample(sm_Trilinear, v.texC);		

	return OutputColor;
			//return float4(1, 0, 0, 1);
}


technique11 DrawDepthMapTech
{

	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
			SetGeometryShader(NULL);
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}

};