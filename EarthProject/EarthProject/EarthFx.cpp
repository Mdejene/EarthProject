#include "EarthFx.h"
#include "Camera.h"

#define SUN_BRIGHT 18.0f
EarthFx::EarthFx()
{
	mFX = 0;
	mTech = 0;
	mfxEarth = 0;
	mfxEarth = 0;
	mfxEarthMap = 0;
	mfxEarthAtmos = 0;
	mfxEarthBump = 0;
	mfxEarthCloud = 0;
	mfxCityLight = 0;
	mfxWVPVar = 0;
	mfxWVar = 0;
	mfxEyeVar = 0;
	mfxLight = 0;
	mVertexLayout = 0;
	mLightMove = 0;
	mfxCloudRot = 0;

	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP);

	mEyePos = D3DXVECTOR3(0, 0, 0);
	
	mLight = Light();

	mRayleighScatteringConstant = 0.025f;
	mMieScatteringConstant = 0.015f;
	mSunBrightness = 18.0f;
	mRayleighScaleDepth = 0.25f;
	mMiePhaseAsymmetryFactor = -0.98f;

	mKr = mRayleighScatteringConstant;
	mKr4PI = mKr * 4.0f * PI;
	mKm = mMieScatteringConstant;
	mKm4PI = mKm * 4.0f * PI;
	mScale = 1 / (mEarthAtmosRadius - mEarthRadius);
	mWaveLength = D3DXVECTOR3( 0.650f, 0.570f, 0.475f );

	mInvWaveLength.x = 1.0f / pow( mWaveLength.x, 4.0f ); // 650 nm for red
	mInvWaveLength.y = 1.0f / pow( mWaveLength.y, 4.0f ); // 570 nm for green
	mInvWaveLength.z = 1.0f / pow( mWaveLength.z, 4.0f ); // 475 nm for blue

	
	mfxOutRadius = 0;
	mfxInnerRadius = 0;
	mfxKrSun = 0;
	mfxKr4Pi = 0;
	mfxKm = 0;
	mfxKm4Pi = 0;
	mfxScale = 0;
	mfxScaleDepth = 0;
	mfxScaleOverScaleDepth= 0;
	mfxSamples= 0;
	mfxG= 0;
	mfxG2= 0;
	mfxInvWaveLength=0;

	AtmosModel = new Model;
	
}

EarthFx::~EarthFx()
{
	mFX->Release();
	mVertexLayout->Release();
	mTech = 0;
	mfxEarth = 0;
	mfxEarthMap = 0;
	mfxEarthMask = 0;
	mfxEarthAtmos = 0;
	mfxEarthBump = 0;
	mfxEarthCloud = 0;
	mfxCityLight = 0;
	mfxWVPVar = 0;
	mfxWVar = 0;
	mfxEyeVar = 0;
	mfxLight = 0;
	mfxCloudRot = 0;

	
	mfxOutRadius = 0;
	mfxInnerRadius = 0;
	mfxKrSun = 0;
	mfxKr4Pi = 0;
	mfxKm = 0;
	mfxKm4Pi = 0;
	mfxScale = 0;
	mfxScaleDepth = 0;
	mfxScaleOverScaleDepth= 0;
	mfxSamples= 0;
	mfxG= 0;
	mfxG2= 0;
	mfxInvWaveLength=0;
	
}

void EarthFx::InitShader(ID3D10Device* Device )
{
	D3DXMatrixIdentity(mEarth.GetEarthModel()->WorldMat());
	md3dDevice = Device;
	mFX = CreateEffect(Device, L"Lighting.fx");
	BuildVertexLayout();
	mEarth.init(Device);
	AtmosModel->init(md3dDevice, EarthM, 5.62*1.125f, D3DXVECTOR3(0, 0, 0), true);	

	mView = GetCamera().view();
	mProj = GetCamera().proj();

	GetCamera().SetPos(D3DXVECTOR3(0, 140.0f, -30)); 
	
	

	mLight.pos		= D3DXVECTOR3(0, 500.0f, 0);
	mLight.dir		= D3DXVECTOR3(0 ,-1.0f, 0);
	mLight.ambient	= D3DXCOLOR(0.001f, 0.001f, 0.001f, 0.001f);
	mLight.diffuse	= D3DXCOLOR(0.42f, 0.42f, 0.42f, 1.0f);
	mLight.specular = D3DXCOLOR(0.84f, 0.84f, 0.84f, 1024);
	mLight.range	= 2000000;
	mLight.att.x	= 0.55f;
	mLight.att.y	= 0.0f;
	mLight.att.z	= 0.0f;
	mLight.spotPow	= 128.0f;
	
	
}

void EarthFx::Render(float dt)
{
	mView = GetCamera().view();
	mProj = GetCamera().proj();
	
	mEyePos = GetCamera().position();
	//md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);
    md3dDevice->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	SetShaderVar();
	mLightMove += dt/100000;
	D3DXVECTOR3 lightPos;
	D3DXVECTOR3 earthMove;;
	lightPos = D3DXVECTOR3(cos(mLightMove) * 1000,0,  sin(mLightMove)*1000);
	D3DXVECTOR3 dir;
	mLight.pos = lightPos;
	D3DXVec3Normalize(&dir, &(-mEyePos));
	mLight.dir = dir;
	if(GetAsyncKeyState('C') & 0x8000)	mLightMove+=0.00051f;
	if(GetAsyncKeyState('V') & 0x8000)	mLightMove-=0.00051f;
	mTech = mFX->GetTechniqueByName("EarthTech");
	D3D10_TECHNIQUE_DESC techDesc;
	
	mTech->GetDesc(&techDesc);

	for(UINT i = 0; i< techDesc.Passes; ++i)
	{
		SetShaderMatrix();
		mTech->GetPassByIndex(i)->Apply(0);
		mEarth.GetEarthModel()->draw(false);
	}

	md3dDevice->IASetInputLayout(mVertexLayout2);
	mTech = mFX->GetTechniqueByName("AtmosTech");
	mTech->GetDesc(&techDesc);

	for(UINT i = 0; i< techDesc.Passes; ++i)
	{
		SetShaderMatrix();
		mTech->GetPassByIndex(i)->Apply(0);
		AtmosModel->draw(true);
	}
	
}

void EarthFx::SetShaderVar()
{
	mfxEarth->SetResource(mEarth.GetEarthTex());
	mfxEarthMap->SetResource(mEarth.GetEarthMapTex());
	mfxEarthMask->SetResource(mEarth.GetEarthMaskTex());
	mfxEarthAtmos->SetResource(mEarth.GetEarthAtmos());
	mfxEarthBump->SetResource(mEarth.GetEarthBumpTex());
	mfxEarthCloud->SetResource(mEarth.GetEarthCloudTex());
	mfxCityLight->SetResource(mEarth.GetEarthCityLightTex());
	mfxLight->SetRawValue(&mLight, 0, sizeof(Light));
	mfxEyeVar->SetFloatVector(GetCamera().position());
	mfxCloudRot->SetRawValue(&mLightMove, 0, sizeof(float));

	//mEarthRadius = 100;
	//mEarthAtmosRadius = mEarthRadius*1.15f;
	//mfxOutRadius->SetFloat(mEarthAtmosRadius);
	//mfxInnerRadius->SetFloat(mEarthRadius);
	//mfxKrSun->SetFloat(mSunBrightness * mKr);
	//mfxKr4Pi->SetFloat(mKr4PI);
	//mfxKm->SetFloat(mSunBrightness * mKm);
	//mfxKm4Pi->SetFloat(mKm4PI);
	//mfxScale->SetFloat(mScale);
	//mfxScaleDepth->SetFloat(mRayleighScaleDepth );
	//mfxScaleOverScaleDepth->SetFloat(mScale/mRayleighScaleDepth);
	//mfxSamples->SetFloat(1.0f);
	//mfxG->SetFloat(mMiePhaseAsymmetryFactor);
	//mfxG2->SetFloat(mMiePhaseAsymmetryFactor*mMiePhaseAsymmetryFactor);
	//mfxInvWaveLength->SetFloatVector(mInvWaveLength);
	//
}

void EarthFx::SetShaderMatrix()
{
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	
	mWVP = m * mView * mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	mfxWVar->SetMatrix((float*)m);
}


ID3D10Effect* EarthFx::CreateEffect(ID3D10Device* Device, std::wstring name)
{
	ID3D10Effect* fx = 0;

	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(name.c_str(), 0, 0, 
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

	mTech = fx->GetTechniqueByName("EarthTech");

	mfxEarth  = fx->GetVariableByName("EarthTex")->AsShaderResource();
	mfxEarthMap  = fx->GetVariableByName("EarthMapTex")->AsShaderResource();
	mfxEarthMask  = fx->GetVariableByName("EarthMaskTex")->AsShaderResource();
	mfxEarthAtmos = fx->GetVariableByName("EarthAtmos")->AsShaderResource();
	mfxEarthBump = fx->GetVariableByName("EarthBumpTex")->AsShaderResource();
	mfxEarthCloud = fx->GetVariableByName("EarthCloudTex")->AsShaderResource();
	mfxCityLight = fx->GetVariableByName("EarthLightTex")->AsShaderResource();
	mfxLight = fx->GetVariableByName("gLight");

	mfxEyeVar = fx->GetVariableByName("gEyePosW")->AsVector();

	mfxWVPVar = fx->GetVariableByName("gWVP")->AsMatrix();
	mfxWVar = fx->GetVariableByName("gWorld")->AsMatrix();
	mfxCloudRot = fx->GetVariableByName("gCloudRot");

	
	/*mfxOutRadius = fx->GetVariableByName("Outradius")->AsScalar();
	mfxInnerRadius = fx->GetVariableByName("InnerRadius")->AsScalar();
	mfxKrSun = fx->GetVariableByName("KrSun")->AsScalar();
	mfxKr4Pi = fx->GetVariableByName("Kr4Pi")->AsScalar();
	mfxKm = fx->GetVariableByName("KmSun")->AsScalar();
	mfxKm4Pi =fx->GetVariableByName("Km4Pi")->AsScalar();
	mfxScale = fx->GetVariableByName("Scale")->AsScalar();
	mfxScaleDepth = fx->GetVariableByName("ScaleDepth")->AsScalar();
	mfxScaleOverScaleDepth = fx->GetVariableByName("ScaleOverScaleDepth")->AsScalar();
	mfxSamples = fx->GetVariableByName("Samples")->AsScalar();
	mfxG = fx->GetVariableByName("G")->AsScalar();
	mfxG2 = fx->GetVariableByName("G2")->AsScalar();
	mfxInvWaveLength = fx->GetVariableByName("InvWaveLength")->AsVector();*/


	return fx;
}

void EarthFx::BuildVertexLayout()
{

	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"DIFFUSE",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0, 56, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
   D3D10_PASS_DESC PassDesc;
   mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
   HR(md3dDevice->CreateInputLayout(vertexDesc, 5, PassDesc.pIAInputSignature,
		   PassDesc.IAInputSignatureSize, &mVertexLayout));

   mTech = mFX->GetTechniqueByName("AtmosTech");
   D3D10_INPUT_ELEMENT_DESC vertexDesc2[] =
	{
		{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},	
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEX", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
   D3D10_PASS_DESC PassDesc2;
   mTech->GetPassByIndex(0)->GetDesc(&PassDesc2);
   HR(md3dDevice->CreateInputLayout(vertexDesc2, 3, PassDesc2.pIAInputSignature,
		   PassDesc2.IAInputSignatureSize, &mVertexLayout2));



}


Earth* EarthFx::GetEarth()
{
	return &mEarth;
}