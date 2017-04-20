
Texture2D Texture : register(t0);


SamplerState smLinear
{
	//Filter   = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;

	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

};



struct PS_IN
{
	float4 posH        : SV_POSITION;
	float4 posWV       : POSITION;
	float4 normalWV    : NORMAL;
	float4 vel         : VELOCITY;
	float4 texC        : TEXCOORD;
};


float4 PS(PS_IN pIn) : SV_Target
{
	return Texture.Sample(smLinear, pIn.texC);

	//return float4(0, 0, 1, 1);
}


