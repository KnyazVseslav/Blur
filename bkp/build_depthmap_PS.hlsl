
struct PS_IN
{
	float4 PosH : SV_POSITION;
	float4 TexC : TEXCOORD;
	float2 depth: DEPTH;
};

float4 PS(PS_IN v): SV_TARGET
{
		//  !!Only for perspective projection matrices, since orthographic projection straight converts points to NDC !!
	return v.depth.x / v.depth.y; // making a homogeneous divide - converting z-coordinate to NDC



/*
	float4 diffuse = ObjectTexture.Sample(samTrilinearAddrUVWrap, v.TexC);

		// Shadows can be cast only by visible pixels;
		// clip discards the current pixel from further processing if x < 0 - therefore if pixel is transparent it'll be discarded

	clip(diffuse.a - 0.15f);
*/

}