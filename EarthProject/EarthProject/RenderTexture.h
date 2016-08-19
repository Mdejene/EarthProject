#ifndef RENDERTEXTURE
#define RENDERTEXTURE
#include <d3d10.h>
#include "d3dUtil.h"
class RenderTexture
{

private:
	ID3D10Device* md3dDevice;
	ID3D10Texture2D* mRenderTargetTex;
	ID3D10RenderTargetView* mRenderTargetView;
	ID3D10ShaderResourceView* mShaderResourceView;
	ID3D10Texture2D* mDepthStencilBuffer;
	ID3D10DepthStencilView* mDepthStencilView;
	D3D10_VIEWPORT mViewport;

	D3DXMATRIX mProjectionMatrix;
	D3DXMATRIX mOrthoMatrix;
public:

	RenderTexture();
	RenderTexture(const RenderTexture& other);
	~RenderTexture();

	void SetUp(ID3D10Device*, int width, int height, float near, float far);

	void SetRenderTarget();
	void ClearRenderTarget();

	void ClearRenderTex();

	void GetOrthoMatrix(D3DXMATRIX&);
	ID3D10ShaderResourceView* GetShaderResourceView();

};
#endif