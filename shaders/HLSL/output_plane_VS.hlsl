
#pragma pack_matrix(row_major)

cbuffer cbPerObject       :  register(b0)
{
	float4x4 WVP;
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
};


VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;

	vOut.posH = vIn.posL;
	vOut.texC = vIn.texC;

	return vOut;
}


