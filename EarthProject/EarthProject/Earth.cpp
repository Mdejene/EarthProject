#include "Earth.h"
#include "d3dUtil.h"

Earth::Earth() 
{
	EarthTex = 0;
	EarthMapTex = 0;
	EarthMaskTex = 0;
	EarthBumpTex = 0;
	EarthAtmos = 0;
	EarthCloudTex = 0;
	EarthCityLightTex = 0;

}

Earth::~Earth()
{
	EarthTex->Release();
	EarthMapTex->Release();
	EarthMaskTex->Release();
	EarthBumpTex->Release();
	EarthAtmos->Release();
	EarthCloudTex->Release();
	EarthCityLightTex->Release();
}

void Earth::Update()
{

	float speed = 0.4f;
	D3DXMATRIX forward, backward, left, right;


	D3DXMatrixTranslation(&forward, 0, 0, speed);
	D3DXMatrixTranslation(&backward, 0, 0, -speed);
	D3DXMatrixTranslation(&left, -speed, 0, 0);
	D3DXMatrixTranslation(&right, speed, 0, 0);
	if(GetAsyncKeyState('U') & 0x8000)	*EarthModel.WorldMat() *= forward;
	if(GetAsyncKeyState('J') & 0x8000)	*EarthModel.WorldMat() *= backward;
	if(GetAsyncKeyState('H') & 0x8000) *EarthModel.WorldMat() *= left;
	if(GetAsyncKeyState('K') & 0x8000)	*EarthModel.WorldMat() *= right;
	if(GetAsyncKeyState('R') & 0x8000) D3DXMatrixIdentity(EarthModel.WorldMat());

}

ID3D10ShaderResourceView* Earth::CreateTex(ID3D10Device* device,char* FileName)
{
	ID3D10ShaderResourceView* tex = 0;
	if(FAILED(D3DX10CreateShaderResourceViewFromFileA(device, FileName, NULL, NULL, &tex, NULL) ) )
	{
			char err[255];
			sprintf_s(err, "Could not load texture %s", FileName);
	}

	return tex;
}
void Earth::init(ID3D10Device* device)
{
	EarthModel.init(device, EarthM, 5.62f, D3DXVECTOR3(0, 0, 0), false);

	EarthTex = CreateTex(device, "EarthTex/earth-living.jpg");
	EarthMapTex = CreateTex(device, "EarthTex/EarthMap_2500x1250.jpg");
	EarthMaskTex =  CreateTex(device, "EarthTex/EarthMask_2500x1250.jpg");
	EarthBumpTex = CreateTex(device,"EarthTex/elev_bump_8k.jpg");
	EarthAtmos = CreateTex(device,"EarthTex/EarthMapAtmos_2500x1250.jpg");
	EarthCloudTex = CreateTex(device,"EarthTex/EarthClouds_2500x1250.jpg");
	EarthCityLightTex = CreateTex(device,"EarthTex/EarthNight_2500x1250.jpg");

	
	
}

Model* Earth::GetEarthModel()
{
	return &EarthModel;
}

ID3D10ShaderResourceView* Earth::GetEarthTex()
{
	return EarthTex;
}
ID3D10ShaderResourceView* Earth::GetEarthMapTex()
{
	return EarthMapTex;
}
ID3D10ShaderResourceView* Earth::GetEarthMaskTex()
{
	return EarthMaskTex;
}
ID3D10ShaderResourceView* Earth::GetEarthBumpTex()
{
	return EarthBumpTex;
}
ID3D10ShaderResourceView* Earth::GetEarthAtmos()
{
	return EarthAtmos;
}
ID3D10ShaderResourceView* Earth::GetEarthCloudTex()
{
	return EarthCloudTex;
}
ID3D10ShaderResourceView* Earth::GetEarthCityLightTex()
{
	return EarthCityLightTex;
}