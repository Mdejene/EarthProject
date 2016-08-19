#ifndef BASIC_SHADER_H
#define BASIC_SHADER_H
#include "FX.h"
#include "d3dUtil.h"

class BasicShader : FX
{
public:
	BasicShader();
	~BasicShader();

	void InitFx(ID3D10Device* Device);
	void SetShaderVars(D3DMATRIX projortho, D3DMATRIX world, D3DMATRIX view, ID3D10ShaderResourceView* tex);
	void GetVarNames();	
	void Render(int indexCount);	
private:
	ID3D10Device*						md3dDevice;

	ID3D10Effect*						mFx;
	ID3D10EffectTechnique*				mTech;
	ID3D10InputLayout*					mVertexLayout;

	ID3D10EffectMatrixVariable*			mfxWorld;
	ID3D10EffectMatrixVariable*			mfxView;
	ID3D10EffectMatrixVariable*			mfxProj;
	ID3D10EffectShaderResourceVariable*	mTex;
};
#endif