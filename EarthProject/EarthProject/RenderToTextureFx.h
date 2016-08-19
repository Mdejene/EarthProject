#ifndef RENDERTOTEXTUREFX_H
#define RENDERTOTEXTUREFX_H
#include <iostream>
#include "d3dUtil.h"
#include <d3d10.h>


class RenderToTextureFx
{
private:
	ID3D10Effect* CreateEffect(std::wstring fxName);
	
	void BuildVertexLayout();


private:

	ID3D10Effect* mFx;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;

	ID3D10EffectMatrixVariable* mfxWorld;
	ID3D10EffectMatrixVariable* mfxView;
	ID3D10EffectMatrixVariable* mfxOrthoProj;

	ID3D10EffectShaderResourceVariable* mfxTexture;

	ID3D10Device* md3dDevice;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWorld;

	ID3D10EffectVariable* mfxDt;
	float dt;
public:
	RenderToTextureFx();
	RenderToTextureFx(const RenderToTextureFx& other);
	~RenderToTextureFx();

	void SetShaderVars(ID3D10ShaderResourceView* tex, D3DMATRIX orthoProj, D3DMATRIX world, D3DMATRIX view);
	void InitShader(ID3D10Device* Device);

	void Render(int indexCount, int iStart, int vStart);
	
	void SetTechnique(char* name);

};
#endif