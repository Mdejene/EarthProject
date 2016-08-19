#include "RenderToTextureFx.h"
#include "Camera.h"
RenderToTextureFx::RenderToTextureFx()
{
	mFx  = 0;
	mTech = 0;
	mVertexLayout = 0;
	mfxTexture = 0;
	mfxWorld = 0;
	mfxView = 0;
	mfxOrthoProj = 0;
	mfxDt = 0;
	dt = 0;

	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWorld);
	
}
RenderToTextureFx::RenderToTextureFx(const RenderToTextureFx& other)
{

}
RenderToTextureFx::~RenderToTextureFx()
{
	mFx->Release();
	mTech = 0;
	mVertexLayout->Release();
	mfxTexture = 0;
	mfxWorld = 0;
	mfxView = 0;
	mfxOrthoProj = 0;
}

void RenderToTextureFx::InitShader(ID3D10Device* Device)
{
	md3dDevice = Device;
	mFx = CreateEffect(L"RenderQuad.fx");
	BuildVertexLayout();

	

}

void RenderToTextureFx::Render(int indexCount, int iStart, int vStart)
{

	D3D10_TECHNIQUE_DESC techniqueDesc;
	
	md3dDevice->IASetInputLayout(mVertexLayout);

    mTech->GetDesc(&techniqueDesc);

	dt += 0.01f;
	for(UINT i=0; i<techniqueDesc.Passes; ++i)
    {
        mTech->GetPassByIndex(i)->Apply(0);
        md3dDevice->DrawIndexed(indexCount, iStart, vStart);
    }
	

	//Cannot be used as shader resource the same time the render target is using it 
	// I set it to null so i get no complainents the next draw call i set the render target
	mfxTexture->SetResource(NULL);
	mTech->GetPassByIndex(0)->Apply(0);
}


ID3D10Effect* RenderToTextureFx::CreateEffect(std::wstring fxName)
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
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &fx, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 
	mTech = fx->GetTechniqueByName("RenderTexture");
	mfxTexture =  fx->GetVariableByName("TEX")->AsShaderResource();
	mfxWorld = fx->GetVariableByName("World")->AsMatrix();
	mfxView = fx->GetVariableByName("View")->AsMatrix();
	mfxOrthoProj = fx->GetVariableByName("OrthoProj")->AsMatrix();
	mfxDt = fx->GetVariableByName("DT")->AsScalar();
	return fx;
}

void RenderToTextureFx::BuildVertexLayout()
{
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
   D3D10_PASS_DESC PassDesc;
   mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
   HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		   PassDesc.IAInputSignatureSize, &mVertexLayout));

}

void RenderToTextureFx::SetShaderVars(ID3D10ShaderResourceView* tex, D3DMATRIX orthoProj, D3DMATRIX world,  D3DMATRIX view)
{


	mfxWorld->SetMatrix((float*)&world);
	mfxView-> SetMatrix((float*)&view);
	mfxOrthoProj->SetMatrix((float*)&orthoProj);
	mTech = mFx->GetTechniqueByName("RenderTexture");
	mfxTexture =  mFx->GetVariableByName("TEX")->AsShaderResource();
	mfxTexture->SetResource(tex);
	mfxDt->SetRawValue(&dt, 0, sizeof(float) );
}


void RenderToTextureFx::SetTechnique(char* name)
{
	mTech = mFx->GetTechniqueByName(name);
}