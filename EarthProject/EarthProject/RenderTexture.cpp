#include "RenderTexture.h"
#include <Windows.h>
RenderTexture::RenderTexture()
{
	md3dDevice = 0;

	mRenderTargetTex = 0;
	mRenderTargetView = 0;

	mShaderResourceView = 0;

	mDepthStencilBuffer = 0;
	mDepthStencilView = 0;

	
	ZeroMemory(&mViewport, sizeof(D3D10_VIEWPORT) );

	D3DXMatrixIdentity(&mProjectionMatrix);
	D3DXMatrixIdentity(&mOrthoMatrix);
	
}


RenderTexture::RenderTexture(const RenderTexture& other)
{

}

RenderTexture::~RenderTexture()
{
	md3dDevice->Release();
	mRenderTargetTex->Release();
	mRenderTargetView->Release();

	mShaderResourceView->Release();

	mDepthStencilBuffer->Release();
	mDepthStencilView->Release();
	
}

void RenderTexture::SetUp(ID3D10Device* device, int width, int height, float cameraNear, float cameraFar)
{
	md3dDevice = device;

	D3D10_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D10_TEXTURE2D_DESC));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D10_USAGE_DEFAULT;
	textureDesc.BindFlags =  D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

	if( FAILED( md3dDevice->CreateTexture2D(&textureDesc, NULL, &mRenderTargetTex) ) )
	{
		MessageBoxA(0, "RENDER TEXTURE.cpp CannotCreate Render Target Tex",0 , 0);
	}

	D3D10_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	//ZeroMemory(&renderTargetViewDesc, sizeof(D3D10_RENDER_TARGET_VIEW_DESC));

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	HRESULT result = md3dDevice->CreateRenderTargetView(mRenderTargetTex, &renderTargetViewDesc, &mRenderTargetView);
	if( FAILED (result) )
	{
		MessageBoxA(0, "RENDER TEXTURE.cpp Cannot Create mRenderTargetView",0 , 0);
	}




	D3D10_SHADER_RESOURCE_VIEW_DESC sRVD;
	//ZeroMemory(&sRVD, sizeof(D3D10_SHADER_RESOURCE_VIEW_DESC) );

	sRVD.Format = textureDesc.Format;
	sRVD.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	sRVD.Texture2D.MipLevels = 1;
	sRVD.Texture2D.MostDetailedMip = 0;

	if( FAILED (md3dDevice->CreateShaderResourceView(mRenderTargetTex, &sRVD, &mShaderResourceView) ) )
	{
		MessageBoxA(0, "RENDER TEXTURE.cpp Cannot Create mShaderResourceView",0 , 0);
	}



	D3D10_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D10_TEXTURE2D_DESC) );

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if( FAILED ( md3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer) ) )
	{
		MessageBoxA(0, "RENDER TEXTURE.cpp Cannot Create mDepthStencilBuffer",0 , 0);
	}



	D3D10_DEPTH_STENCIL_VIEW_DESC dSVD;
	ZeroMemory(&dSVD, sizeof(dSVD) );

	dSVD.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dSVD.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	dSVD.Texture2D.MipSlice = 0;

	if( FAILED ( md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, &dSVD, &mDepthStencilView) ) )
	{
		MessageBoxA(0, "RENDER TEXTURE.cpp Cannot Create mDepthStencilView",0 , 0);
	}



	mViewport.Width = width;
    mViewport.Height = height;
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;

	D3DXMatrixPerspectiveFovLH(&mProjectionMatrix, ((float)D3DX_PI / 4.0f), ((float)width / (float)height), cameraNear, cameraFar);

	D3DXMatrixOrthoLH(&mOrthoMatrix, (float)width, (float)height, cameraNear, cameraFar);


}

void RenderTexture::SetRenderTarget()
{
	md3dDevice->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	md3dDevice->RSSetViewports(1, &mViewport);


}

void RenderTexture::ClearRenderTarget()
{
	float color[4];
	for(int i = 0; i < 4; i++)
	{
		color[i] = 0.0f;
	}
	md3dDevice->ClearRenderTargetView(mRenderTargetView, color);
	md3dDevice->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

}

void RenderTexture::GetOrthoMatrix(D3DXMATRIX& getOrtho)
{
	getOrtho = mOrthoMatrix;	
}

ID3D10ShaderResourceView* RenderTexture::GetShaderResourceView()
{ 
	return mShaderResourceView; 
}

void RenderTexture::ClearRenderTex()
{
	mShaderResourceView = 0;
}