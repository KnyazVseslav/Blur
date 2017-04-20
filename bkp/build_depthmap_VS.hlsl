
#pragma pack_matrix(row_major)


cbuffer cbPerFrame       :  register(b0)
{
	float4x4 LightWVP;
};



Texture2D ObjectTexture  :  register(t0); // возможно надо перекинуть в build_depthmap_PS.hlsl



SamplerState samTrilinearAddrUVWrap // возможно надо перекинуть в build_depthmap_PS.hlsl
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_IN
{
	float4 lPos:  POSITION;
	float4 lNorm: NORMAL;
	float4 vel:   VELOCITY;
	float4 TexC:  TEXCOORD;
};


struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float4 TexC : TEXCOORD;
	float2 depth: DEPTH;
};


VS_OUT VS(VS_IN vIn)
{

	VS_OUT vOut;

	
 vOut.PosH = mul(vIn.lPos, LightWVP); // перевод точек сцены к видовому и проекционному пространствам источника света	
	
		//  !!Only for perspective projection matrices, since orthographic projection straight converts points to NDC !!
	vOut.depth.xy = vOut.PosH.zw; // saving depth relative to LS frame (now stored in z) and point's
								  // initial (that was before LightWVP transform) z coordinate (now stored in w);
								  // then in PS we'll make a transform to NDC space dividing z by w (perspective divide) 
							                


		vOut.TexC = vIn.TexC; // texture coordinates left unchanged


	return vOut;

}


