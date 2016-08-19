#include "FX.h"

FX::FX()
{
}

FX::~FX()
{

}

ID3D10Effect* FX::CreateEffect(ID3D10Device* Device, std::wstring fxName)
{
	ID3D10Effect* fx = 0;
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
  shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(fxName.c_str(), 0, 0, 
		"fx_4_0", shaderFlags, 0, Device, 0, 0, &fx, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	return fx;
}


ID3D10InputLayout* FX::BuildVertexLayout(ID3D10EffectTechnique* Technique, ID3D10Device* device)
{
	ID3D10InputLayout* vertexLayout = 0;
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"DIFFUSE",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 56, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
   D3D10_PASS_DESC PassDesc;
   Technique->GetPassByIndex(0)->GetDesc(&PassDesc);
   HR(device->CreateInputLayout(vertexDesc, 5, PassDesc.pIAInputSignature,
		   PassDesc.IAInputSignatureSize, &vertexLayout));

   return vertexLayout;
}
ID3D10InputLayout* FX::BuildPosTexVertexLayout(ID3D10EffectTechnique* Technique, ID3D10Device* device)
{
	ID3D10InputLayout* vertexLayout = 0;
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	    0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,	    0, 20, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
   D3D10_PASS_DESC PassDesc;
   Technique->GetPassByIndex(0)->GetDesc(&PassDesc);
   HR(device->CreateInputLayout(vertexDesc, 3, PassDesc.pIAInputSignature,
		   PassDesc.IAInputSignatureSize, &vertexLayout));

   return vertexLayout;
}



ID3D10InputLayout* FX::BuildGrassVertexLayout(ID3D10EffectTechnique* Technique, ID3D10Device* device)
{
	ID3D10InputLayout* vertexLayout = 0;
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	    0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"VPOS",     0, DXGI_FORMAT_R32G32B32_FLOAT,	1, 0, D3D10_INPUT_PER_INSTANCE_DATA, 1}
	};

	// Create the input layout
   D3D10_PASS_DESC PassDesc;
   Technique->GetPassByIndex(0)->GetDesc(&PassDesc);
   HR(device->CreateInputLayout(vertexDesc, 3, PassDesc.pIAInputSignature,
		   PassDesc.IAInputSignatureSize, &vertexLayout));

   return vertexLayout;
}



ID3D10InputLayout* FX::BuildSkyVertexLayout(ID3D10EffectTechnique* Technique, ID3D10Device* device)
{
	ID3D10InputLayout* vertexLayout = 0;
	 D3D10_INPUT_ELEMENT_DESC vertexDesc2[] =
	{
		{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},	
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEX", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
   D3D10_PASS_DESC PassDesc2;
   Technique->GetPassByIndex(0)->GetDesc(&PassDesc2);
   HR(device->CreateInputLayout(vertexDesc2, 3, PassDesc2.pIAInputSignature,
		   PassDesc2.IAInputSignatureSize, &vertexLayout));
   return vertexLayout;
}


void FX::InitFx()
{
}
void FX::Render( )
{
}
void FX::setShaderVars()
{
}
void FX::GetVarNames()
{
}
	



