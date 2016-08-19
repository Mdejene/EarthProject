#include "Blur.h"
#include "d3dUtil.h"
#include <math.h>
#include <D3DX10math.h>
BlurFx::BlurFx()
{
	mFx  = 0;
	mTech = 0;
	mVertexLayout = 0;
	md3dDevice = 0;

	mfxWeights = 0;
	mfxOffsetX = 0;
	mfxOffsetY = 0;
	mfxTexture = 0;

	float sigma = 0;
	
	int amount = 0;
	float texWidth = 0;
	float texHeight = 0;

	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWorld);
	for(int i = 0; i < 4; i++)
	{
		sWeight[i] = D3DXVECTOR4(0, 0, 0, 0);
		sOffsetX[i] = D3DXVECTOR4(0, 0, 0, 0);
		sOffsetY[i] = D3DXVECTOR4(0, 0, 0, 0);
	}
}

BlurFx::~BlurFx()
{

	mFx->Release();
	mTech = 0;

	mVertexLayout->Release();	
	mfxTexture = 0;
	mfxWorld = 0;
	mfxView = 0;
	mfxProj = 0;

	mfxWeights = 0;
	mfxOffsetX = 0;
	mfxOffsetY = 0;
}

void BlurFx::InitShader(ID3D10Device* Device, int radius, int blurAmount, float textureWidth, float textureHeight)
{
	md3dDevice = Device;
	this->radius = radius;
	amount = blurAmount;
	texWidth = textureWidth ;
	texHeight = textureHeight;

	mFx = CreateEffect(L"Blur.fx");
	BuildVertexLayout();

	for(int i = 0; i < KERNAL; i++)
	{
		weights[i] = NULL;
		offsetX[i] = NULL;
		offsetY[i] = NULL;
	}

	sigma = 0;

	
	CalculateKernals();
	CalculateOffset();
}

void BlurFx::Render(int indexCount)
{

	
	D3D10_TECHNIQUE_DESC techniqueDesc;

	md3dDevice->IASetInputLayout(mVertexLayout);

	mTech->GetDesc(&techniqueDesc);

	for(UINT i= 0; i < techniqueDesc.Passes; i++)
	{
		mTech->GetPassByIndex(i)->Apply(i);
		md3dDevice->DrawIndexed(indexCount, 0, 0);
	}

	mfxTexture->SetResource(NULL);
	mTech->GetPassByIndex(0)->Apply(0);

}


void BlurFx::CalculateKernals()
{
	sigma = RADIUS / amount;

	float tSigmaSq = 2.0f * sigma * sigma;
	float sigmaRoot = (float)sqrt(tSigmaSq * PI);
	float total = 0;
	float distance = 0;

	int index = 0;

	for(int i = -RADIUS; i <= RADIUS; ++i)
	{
		distance = i * i;
		index = i + RADIUS;
		weights[index] = (float)exp(-distance / tSigmaSq) / sigmaRoot;
		total += weights[index];
	}

	//normalizes values;
	for(int i = 0 ; i < KERNAL; ++ i)
		weights[i] /= total;


	

}

void BlurFx::CalculateOffset()
{
	int index = 0;
	float xOff = 1.0f / texWidth;
	float yOff = 1.0f / texHeight;

	for(int i = -RADIUS; i <= RADIUS; i++)
	{
		index = i + RADIUS;
		offsetX[index] = i * xOff;
		offsetY[index] =  i * yOff;
	}

}

ID3D10Effect* BlurFx::CreateEffect(std::wstring fxName)
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


	mTech = fx->GetTechniqueByName("Blur");
	mfxTexture =  fx->GetVariableByName("Tex")->AsShaderResource();

	mfxWorld = fx->GetVariableByName("World")->AsMatrix();
	mfxView = fx->GetVariableByName("View")->AsMatrix();
	mfxProj = fx->GetVariableByName("Proj")->AsMatrix();

	
	mfxWeights = fx->GetVariableByName("weights")->AsScalar();
	mfxOffsetX = fx->GetVariableByName("oX")->AsScalar();
	mfxOffsetY = fx->GetVariableByName("oY")->AsScalar();


	return fx;
}

void BlurFx::BuildVertexLayout()
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

void BlurFx::SetShaderVars(ID3D10ShaderResourceView* tex, D3DMATRIX orthoProj)
{



	D3DXMATRIX  viewq;
	D3DXMatrixIdentity(&viewq);
	D3DXMatrixLookAtLH(&viewq, &D3DXVECTOR3(0, 0,-1), &D3DXVECTOR3(0, 0,0), &D3DXVECTOR3(0,1 ,0));

	mfxWorld->SetMatrix((float*)&mWorld);
	mfxView-> SetMatrix((float*)&viewq);
	mfxProj->SetMatrix((float*)&orthoProj);
		
	
	
	mfxWeights->SetFloatArray((float*)weights, 0, KERNAL ); 
	mfxOffsetX->SetFloatArray((float*)offsetX, 0,KERNAL ); 
	mfxOffsetY->SetFloatArray((float*)offsetY, 0, KERNAL ); 
	
	
	mfxTexture->SetResource(tex);
	mTech = mFx->GetTechniqueByName("Blur");
}

void BlurFx::SetTech(char * tech)
{
	mTech = mFx->GetTechniqueByName(tech);
}
