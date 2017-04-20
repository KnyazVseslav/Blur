#include "Blur.h"

#include "ShaderSystem.h"

using namespace effects::shader_system;


void Blur::create_horizontal_tex_SRV_UAV_RTV_DSV(UINT Width, UINT Height)
{

	ReleaseCOM(SRV_h);
	ReleaseCOM(UAV_h);
	ReleaseCOM(RTV);

	this->Width = Width;
	this->Height = Height;

	
	texdesc.Width = Width;
	texdesc.Height = Height;
	texdesc.MipLevels = 1;
	texdesc.ArraySize = 1;
	texdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texdesc.SampleDesc.Count = 1;
	texdesc.SampleDesc.Quality = 0;
	texdesc.Usage = D3D11_USAGE_DEFAULT; 

	texdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;

	texdesc.CPUAccessFlags = 0;

	texdesc.MiscFlags = 0;

	// creating tex for horizontal blur
	HR(d3dDevice->CreateTexture2D(&texdesc, 0, &tx_blur_horizontal));

	HR(d3dDevice->CreateShaderResourceView(tx_blur_horizontal, 0, &SRV_h));
	HR(d3dDevice->CreateRenderTargetView(tx_blur_horizontal, 0, &RTV));
	HR(d3dDevice->CreateUnorderedAccessView(tx_blur_horizontal, 0, &UAV_h));
	

	ReleaseCOM(tx_blur_horizontal);

}

void Blur::OMbind_DSV_RTV_VP_of_horizontal_tex()
{
	// setting

	float R, G, B;

	R = float(74) / 255;
	G = float(206) / 255;
	B = float(255) / 255;


	float COLOR[4] = { 0, 0, 1, 0 };

	ID3D11RenderTargetView * RTVs[1] = { RTV };

	d3dDeviceContext->OMSetRenderTargets(1, RTVs, DSV);


	// clearing

	d3dDeviceContext->ClearRenderTargetView(RTV, COLOR);
	d3dDeviceContext->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void Blur::create_vertical_tex_SRV_UAV()
{
	
	ReleaseCOM(SRV_v);
	ReleaseCOM(UAV_v);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = Width;
	texDesc.Height = Height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	
	HR(d3dDevice->CreateTexture2D(&texDesc, 0, &tx_blur_vertical));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	HR(d3dDevice->CreateShaderResourceView(tx_blur_vertical, &srvDesc, &SRV_v));

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	HR(d3dDevice->CreateUnorderedAccessView(tx_blur_vertical, &uavDesc, &UAV_v));

	ReleaseCOM(tx_blur_vertical);

}


void Blur::apply(UINT blur_passes)
{

	ShaderSystem FX;

	FX.Init("BlurFX", d3dDevice, d3dDeviceContext);


	FX.AddShader("CS_blur_h", ComputeShader, "shaders\\HLSL\\blur_HorizontalPass_CS.hlsl", "CS_HorizontalBlurPass", "cs_5_0");
	FX.AddShader("CS_blur_v", ComputeShader, "shaders\\HLSL\\blur_VerticalPass_CS.hlsl", "CS_VerticalBlurPass", "cs_5_0");

	FX.shaders["CS_blur_h"].AddTexture2D(SRV_h, 0);
	FX.shaders["CS_blur_h"].AddUAV(UAV_v, 0);


	FX.shaders["CS_blur_v"].AddTexture2D(SRV_v, 0);
	FX.shaders["CS_blur_v"].AddUAV(UAV_h, 0);


	for (int i = 0; i < blur_passes; i++)
	{

		// Горизонтальный проход размытия

		FX.shaders["CS_blur_h"].set();

		UINT ThreadGroups_qty_along_X = (UINT)ceilf(Width / 256.0f);

		d3dDeviceContext->Dispatch(ThreadGroups_qty_along_X, Height, 1);


		ID3D11ShaderResourceView* null_SRV[1] = { 0 };
		d3dDeviceContext->CSSetShaderResources(0, 1, null_SRV);

		ID3D11UnorderedAccessView* null_UAV[1] = { 0 };
		d3dDeviceContext->CSSetUnorderedAccessViews(0, 1, null_UAV, 0);




		// Вертикальный проход размытия

		FX.shaders["CS_blur_v"].set();

		UINT ThreadGroups_qty_along_Y = (UINT)ceilf(Height / 256.0f);

		d3dDeviceContext->Dispatch(Width, ThreadGroups_qty_along_Y, 1);


		d3dDeviceContext->CSSetShaderResources(0, 1, null_SRV);
		d3dDeviceContext->CSSetUnorderedAccessViews(0, 1, null_UAV, 0);

	}



	d3dDeviceContext->CSSetShader(0, 0, 0);



}





ID3D11ShaderResourceView*  Blur::Get_SRV_h()
{
	return SRV_h;
}

ID3D11ShaderResourceView*  Blur::Get_SRV_v()
{
	return SRV_v;
}

ID3D11UnorderedAccessView*  Blur::Get_UAV_h()
{
	return UAV_h;
}
ID3D11UnorderedAccessView*  Blur::Get_UAV_v()
{
	return UAV_h;
}

ID3D11RenderTargetView* Blur::Get_RTV()
{
	return RTV;
}


void Blur::Set_DSV(ID3D11DepthStencilView* DSV)
{
	this->DSV= DSV;
}
