#pragma once

#ifndef BLUR_H
#define BLUR_H


#include "D3D.h"

class Blur
{

private:

	// For horizontal pass
	D3D11_TEXTURE2D_DESC texdesc;
	ID3D11Texture2D* tx_blur_horizontal;
	ID3D11DepthStencilView* DSV; // DSV
	ID3D11ShaderResourceView* SRV_h; // SRV
	ID3D11UnorderedAccessView*  UAV_h; // UAV
	ID3D11RenderTargetView* RTV;  // RTV

	D3D11_VIEWPORT VP;


	// For vertical pass
	ID3D11Texture2D * tx_blur_vertical;
	ID3D11ShaderResourceView* SRV_v; // SRV
	ID3D11UnorderedAccessView*  UAV_v; // UAV


	UINT Width;
	UINT Height;


	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;

public:

	Blur() : d3dDevice(0), d3dDeviceContext(0), DSV(0), SRV_h(0), SRV_v(0), UAV_h(0), UAV_v(0) {}
	~Blur(){ ReleaseCOM(DSV); ReleaseCOM(SRV_h); ReleaseCOM(SRV_v); ReleaseCOM(UAV_h); ReleaseCOM(UAV_v); ReleaseCOM(RTV); }


public:


	void Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
	{
		d3dDevice = Device; d3dDeviceContext = DeviceContext;
	}


	void create_horizontal_tex_SRV_UAV_RTV_DSV(UINT Width, UINT Height);

	void OMbind_DSV_RTV_VP_of_horizontal_tex();


	void create_vertical_tex_SRV_UAV();

	

	void apply(UINT blur_passes = 2);

	ID3D11ShaderResourceView*  Get_SRV_h();
	ID3D11ShaderResourceView*  Get_SRV_v();

	ID3D11UnorderedAccessView*  Get_UAV_h();
	ID3D11UnorderedAccessView*  Get_UAV_v();

	ID3D11RenderTargetView*  Get_RTV();

    void  Set_DSV(ID3D11DepthStencilView* DSV);
	ID3D11DepthStencilView*  Get_DSV();


};




#endif

