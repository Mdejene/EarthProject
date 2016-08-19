#ifndef FX_H
#define FX_H
#include "d3dUtil.h"
#include <d3d10.h>
#include <iostream>
#include <string>

using namespace std;


///<summary> ----------------------------------EFFECTS BASE CLASS------------------------------------
/// effects base class hold all functions 
///	static to create an fx class
/// also includes virtual functions to what an
/// fx class will hold
///</summary>

class FX
{
public:
	static ID3D10Effect* CreateEffect(ID3D10Device* Device, std::wstring fxName);
	static ID3D10InputLayout* BuildVertexLayout(ID3D10EffectTechnique* Technique, ID3D10Device* device);
	static ID3D10InputLayout* BuildPosTexVertexLayout(ID3D10EffectTechnique* Technique, ID3D10Device* device);
	static ID3D10InputLayout* BuildGrassVertexLayout(ID3D10EffectTechnique* Technique, ID3D10Device* device);
	static ID3D10InputLayout* BuildSkyVertexLayout(ID3D10EffectTechnique* Technique, ID3D10Device* device);
public:
	FX();
	~FX();

	virtual void InitFx(); 
	virtual void Render( );
	virtual void setShaderVars(); 
	virtual void GetVarNames();	
};

#endif