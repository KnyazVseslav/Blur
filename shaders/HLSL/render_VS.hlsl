
#pragma pack_matrix(row_major)


cbuffer cbPerScene         :  register(b0)
{
	float4x4 WV;
	float4x4 View;

	float4x4 LightWVP; // for shadow mapping
	
};


cbuffer cbPerObject       :  register(b1)
{
	float4x4 World;
	float4x4 LWVP;
	float4x4 TexMtx;
};



struct VS_IN
{
	float4 posL    : POSITION;
	float4 normalL : NORMAL;
	float4 vel     : VELOCITY;
	float4 texC    : TEXCOORD;
};

struct VS_OUT
{		
	float4 posH        : SV_POSITION;
	float4 posWV       : POSITION;
	float4 normalWV    : NORMAL;
	float4 vel         : VELOCITY;
	float4 texC        : TEXCOORD;	

	float4 PointInLS_H : TEXCOORD1; // for shadow mapping
};



VS_OUT VS(VS_IN vIn)
{

	VS_OUT vOut;
	
	
	
	vOut.posWV = mul(vIn.posL, WV);
	vOut.normalWV = mul(vIn.normalL, WV);

	 
	vOut.posH = mul(vIn.posL, LWVP);
	 
	 vOut.texC = mul(vIn.texC, TexMtx);
	
	
	 vOut.PointInLS_H = mul(vIn.posL, LightWVP); 



	 return vOut;

}


