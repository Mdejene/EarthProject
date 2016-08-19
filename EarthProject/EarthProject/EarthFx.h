#ifndef EARTHFX_H
#define EARTHFX_H
#include "d3dUtil.h"
#include <d3d10.h>
#include "Earth.h"
#include <iostream>
#include "Light.h"
#include "Model.h"



class EarthFx
{
private:

	ID3D10Effect* CreateEffect(ID3D10Device* Device, std::wstring fxName);

	void SetShaderVar();
	void SetShaderMatrix();
	void BuildVertexLayout();
public:
	Earth mEarth;
private:
	
	Light mLight;

	float mLightMove;

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10InputLayout* mVertexLayout2;

	ID3D10EffectMatrixVariable* mfxWVPVar; 
	ID3D10EffectMatrixVariable* mfxWVar; 
	ID3D10EffectVectorVariable* mfxEyeVar;
	ID3D10EffectVariable* mfxLight;
	
	ID3D10EffectVariable* mfxCloudRot;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;
	D3DXVECTOR3 mEyePos;

	ID3D10EffectShaderResourceVariable* mfxEarth;
	ID3D10EffectShaderResourceVariable* mfxEarthMap;
	ID3D10EffectShaderResourceVariable* mfxEarthMask;
	ID3D10EffectShaderResourceVariable* mfxEarthAtmos;
	ID3D10EffectShaderResourceVariable* mfxEarthBump;
	ID3D10EffectShaderResourceVariable* mfxEarthCloud;
	ID3D10EffectShaderResourceVariable* mfxCityLight;
	
	ID3D10EffectScalarVariable* mfxmKr;
	ID3D10EffectScalarVariable* mfxOutRadius;
	ID3D10EffectScalarVariable* mfxInnerRadius;
	ID3D10EffectScalarVariable* mfxKrSun;
	ID3D10EffectScalarVariable* mfxKr4Pi;
	ID3D10EffectScalarVariable* mfxKm;
	ID3D10EffectScalarVariable* mfxKm4Pi;
	ID3D10EffectScalarVariable* mfxScale;
	ID3D10EffectScalarVariable* mfxScaleDepth;
	ID3D10EffectScalarVariable* mfxScaleOverScaleDepth;
	ID3D10EffectScalarVariable* mfxSamples;
	ID3D10EffectScalarVariable* mfxG;
	ID3D10EffectScalarVariable* mfxG2;
	ID3D10EffectVectorVariable* mfxInvWaveLength;
	ID3D10Device* md3dDevice;


	float mEarthRadius;
	float mEarthAtmosRadius;

	float mRayleighScatteringConstant;
	float mMieScatteringConstant;
	float mSunBrightness;
	float mRayleighScaleDepth;
	float mMiePhaseAsymmetryFactor ;

	D3DXVECTOR3 mWaveLength;

	float mKr;
	float mKr4PI;
	float mKm;
	float mKm4PI;
	float mScale;

	D3DXVECTOR3 mInvWaveLength;
	
public:
	EarthFx();
	~EarthFx();
	void InitShader(ID3D10Device* Device);
	
	void Render(float dt);

	Earth* GetEarth();
	Model* AtmosModel;
};
#endif