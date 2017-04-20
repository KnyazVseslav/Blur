#include "lighthelper.hlsl"

#pragma pack_matrix(row_major)


static const float DEPTH_EPSILON = 0.001f;
static const float DEPTHMAP_SIZE = 2048.0f;
static const float DEPTHMAP_DX = 1.0f / DEPTHMAP_SIZE;


cbuffer cbPerFrame     :  register(b0)
{	
	float4x4 WV;
	
	float4 LightDir;
	float4 LightPos;
	float4 pos_test2;

	float4 L_ambient;
	float4 L_diffuse;
	float4 L_specular;

	int LightSourceType; 
	LightSource light;
	LightSource light2;
	float4 EyePosW;

};


RWTexture2D<float> mask_buffer : register(u1);


Texture2D DiffuseMap   :  register(t1);



SamplerState sm_Trilinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;	
};

// for shadow mapping
SamplerState sm_Shadow
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};


struct PS_IN
{		
	float4 posH        : SV_POSITION;
	float4 posWV       : POSITION;
	float4 normalWV    : NORMAL;
	float4 vel         : VELOCITY;
	float4 texC        : TEXCOORD;	

	float4 PointInLS_H : TEXCOORD1; // for shadow mapping
};


float4 PS(PS_IN pIn): SV_TARGET
{

	// Get materials from texture maps
	float4 diffuse  = DiffuseMap.Sample(sm_Trilinear, pIn.texC);
	float4 specular = diffuse;
	

	// Map [0, 1] --> [0, 256]
	specular.a *= 256.0f;

	
	float4 n = normalize(pIn.normalWV);

	
		SurfaceInfo v = {pIn.posWV, n, diffuse, specular};

	
	float shadow_factor = 1.0f;

	
	float4 litColor;


	if (1 == LightSourceType)
		litColor = ParallelLight(v, light, LightDir, EyePosW, WV, shadow_factor);		 
	
	if (2 == LightSourceType)
		 litColor = PointLight(v, light, LightPos, pos_test2, EyePosW, WV, shadow_factor, L_ambient, L_diffuse, L_specular);

	if (3 == LightSourceType)
			 litColor = Spotlight(v, light, LightDir, LightPos, pos_test2, EyePosW, WV, shadow_factor, L_ambient, L_diffuse, L_specular);

	


	   
	return litColor;
	

	
}




