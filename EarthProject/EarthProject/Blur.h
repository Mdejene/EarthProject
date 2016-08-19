#ifndef BLUR_H
#define BLUR_H
#include <iostream>
#include "d3dUtil.h"
#include <d3d10.h>

#define RADIUS 7
#define KERNAL (RADIUS * 2 +1)

class BlurFx
{
private:
	ID3D10Effect* CreateEffect(std::wstring fxName);
	void BuildVertexLayout();

	void CalculateOffset();
	void CalculateKernals();
	
private:
		
	float sigma;
	int radius;
	int amount;
	float texWidth;
	float texHeight;

	D3DXVECTOR4 sWeight[4];
	D3DXVECTOR4 sOffsetX[4];
	D3DXVECTOR4 sOffsetY[4];
	

	float weights[KERNAL];
	float offsetX[KERNAL];
	float offsetY[KERNAL];

	ID3D10Device* md3dDevice;

	ID3D10Effect* mFx;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;


	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWorld;

	ID3D10EffectMatrixVariable* mfxWorld;
	ID3D10EffectMatrixVariable* mfxView;
	ID3D10EffectMatrixVariable* mfxProj;

	
	ID3D10EffectScalarVariable* mfxWeights;
	ID3D10EffectScalarVariable* mfxOffsetX;
	ID3D10EffectScalarVariable* mfxOffsetY;

	ID3D10EffectShaderResourceVariable* mfxTexture;

public:
	BlurFx();
	~BlurFx();

	void InitShader(ID3D10Device* Device, int radius, int blurAmount, float textureWidth, float textureHeight);
	void Render(int indexCount);
	void SetShaderVars(ID3D10ShaderResourceView* tex, D3DMATRIX orthoProj);
	void SetTech(char*);

};
#endif