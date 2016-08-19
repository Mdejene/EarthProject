#include "BasicShader.h"

BasicShader::BasicShader()
{
	md3dDevice = 0;
	
	mFx = 0;
	mTech = 0;
	mVertexLayout = 0;
	
	mfxWorld = 0;
	mfxView = 0;
	mfxProj = 0;
	mTex = 0;
}

BasicShader::~BasicShader()
{
	md3dDevice = 0;
	
	mFx = 0;
	mTech = 0;
	mVertexLayout = 0;
	
	mfxWorld = 0;
	mfxView = 0;
	mfxProj = 0;
	mTex = 0;
}

void BasicShader::InitFx(ID3D10Device* Device)
{
	md3dDevice = Device;
	mFx = FX::CreateEffect(md3dDevice, L"BasicShader.fx");
	GetVarNames();
	mVertexLayout = FX::BuildPosTexVertexLayout(mTech, md3dDevice);


}

void BasicShader::SetShaderVars(D3DMATRIX proj, D3DMATRIX world, D3DMATRIX view, ID3D10ShaderResourceView* tex)
{
	mfxWorld->SetMatrix((float*)&world);
	mfxView->SetMatrix((float*)&view);
	mfxProj->SetMatrix((float*)&proj);
	mTex->SetResource(tex);
	

}
void BasicShader::GetVarNames()
{
	mTech = mFx->GetTechniqueByName("BasicShader");
	mfxWorld = mFx->GetVariableByName("world")->AsMatrix();
	mfxView = mFx->GetVariableByName("view")->AsMatrix();
	mfxProj = mFx->GetVariableByName("proj")->AsMatrix();
	mTex = mFx->GetVariableByName("Test")->AsShaderResource();

}
void BasicShader::Render(int indexCount)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;	
	md3dDevice->IASetInputLayout(mVertexLayout);
    mTech->GetDesc(&techniqueDesc);	
	

	for(UINT i=0; i<techniqueDesc.Passes; ++i)
    {
        mTech->GetPassByIndex(i)->Apply(0);
        md3dDevice->DrawIndexed(indexCount, 0, 0);
    }
	
}