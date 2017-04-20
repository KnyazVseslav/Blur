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



Texture2D depthMap     :  register(t0); // for shadow mapping - gets assigned with depth map SRV

Texture2D DiffuseMap   :  register(t1);
Texture2D SpecularMap  :  register(t2);

Texture2D OverlayMap   :  register(t3);



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


float CalcShadowFactor(float4 PointInLS_H)
{
	
	PointInLS_H.xyz /= PointInLS_H.w;

	
	if( PointInLS_H.x < -1.0f || PointInLS_H.x > 1.0f || 
	    PointInLS_H.y < -1.0f || PointInLS_H.y > 1.0f ||
	    PointInLS_H.z < 0.0f )
	    return 0.0f;
	  
	// Transform from light source projection NDC (x = -1..1, y = -1..1) space
	// to texture space (u = 0..1, v = 0..1) of a depthMap
	PointInLS_H.x = +0.5f*PointInLS_H.x + 0.5f;
	PointInLS_H.y = -0.5f*PointInLS_H.y + 0.5f;
	
	// Depth in NDC space (0..1)
	float depth = PointInLS_H.z;

	
	
	// Sample shadow map to get nearest depth to scene point (uv)
	float s0 = depthMap.Sample(sm_Shadow, PointInLS_H.xy).r;
	float s1 = depthMap.Sample(sm_Shadow, PointInLS_H.xy + float2(DEPTHMAP_DX, 0)).r;
	float s2 = depthMap.Sample(sm_Shadow, PointInLS_H.xy + float2(0, DEPTHMAP_DX)).r;
	float s3 = depthMap.Sample(sm_Shadow, PointInLS_H.xy + float2(DEPTHMAP_DX, DEPTHMAP_DX)).r;
	
	

		/*
	
		----s0*----------s1*-----
			  |			   |
			  |	 *(u, v)   |
			  |		depth  |
			  |			   |
		----s2*----------s3*-----
	
		*/

	float result0 = depth <= s0 + DEPTH_EPSILON;
	float result1 = depth <= s1 + DEPTH_EPSILON;
	float result2 = depth <= s2 + DEPTH_EPSILON;
	float result3 = depth <= s3 + DEPTH_EPSILON;	


	
	float2 texelPos = DEPTHMAP_SIZE*PointInLS_H.xy;
	
	float2 t = frac( texelPos );
	
	

 return lerp( lerp(result0, result1, t.x), lerp(result2, result3, t.x), // interpolate along x-axis first between s0 and s1, then between s2 and s3
	               t.y ); // and then - along y-axis between two previous lerps

	/*

	 -----*----s0s1*---*-----
	      |		   |   |
		  |	*(u,v) |   |
		  |		   * lerp(s0s1, s2s3, t.y) 
		  |		   |   |
	------*----s2s3*---*-----

	*/
	
				  
}

bool Equal(float4x4 m1, float4x4 m2)
{

	for(dword i= 0; i < 4; i++)
	 for(dword j= 0; j < 4; j++)
	   if(m1[i][j] != m2[i][j]) return false;

	return true;

}


float4 PS(PS_IN pIn): SV_TARGET
{

	//Get materials from texture maps
	float4 diffuse  = DiffuseMap.Sample(sm_Trilinear, pIn.texC);
	float4 specular = diffuse;
	

	//Map [0, 1] --> [0, 256]
	specular.a *= 256.0f;

	
	float4 n = normalize(pIn.normalWV);

	
		SurfaceInfo v = {pIn.posWV, n, diffuse, specular};

	
	float shadow_factor = CalcShadowFactor(pIn.PointInLS_H);

	
	float4 litColor;


	if (1 == LightSourceType)
		litColor = ParallelLight(v, light, LightDir, EyePosW, WV, shadow_factor);		 
		//litColor = ParallelLight(v, light, LightDir, EyePosW, WV);
	
	if (2 == LightSourceType)
		 litColor = PointLight(v, light, LightPos, pos_test2, EyePosW, WV, shadow_factor, L_ambient, L_diffuse, L_specular);

	if (3 == LightSourceType)
		// litColor = Spotlight(v, light, EyePosW, WV);
			 litColor = Spotlight(v, light, LightDir, LightPos, pos_test2, EyePosW, WV, shadow_factor, L_ambient, L_diffuse, L_specular);

	


		

	litColor.a= diffuse.a;

	   
	return litColor;
	

	
}




