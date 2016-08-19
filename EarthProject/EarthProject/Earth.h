#ifndef EARTH_H
#define EARTH_H
#include "Model.h"
#include "Vertex.h"

class Earth
{

private:
	ID3D10ShaderResourceView* CreateTex(ID3D10Device* device, char* FileName);
	
	std::vector<Vertex> vNormal;

private:
	ID3D10ShaderResourceView* EarthTex;
	ID3D10ShaderResourceView* EarthMapTex;
	ID3D10ShaderResourceView* EarthMaskTex;
	ID3D10ShaderResourceView* EarthBumpTex;
	ID3D10ShaderResourceView* EarthAtmos;
	ID3D10ShaderResourceView* EarthCloudTex;
	ID3D10ShaderResourceView* EarthCityLightTex;

	Model EarthModel;

	

	
public:
	Earth();
	~Earth();

	Model* GetEarthModel();
	void init(ID3D10Device* device);

	void Update();

	ID3D10ShaderResourceView* GetEarthTex();
	ID3D10ShaderResourceView* GetEarthMapTex();
	ID3D10ShaderResourceView* GetEarthMaskTex();
	ID3D10ShaderResourceView* GetEarthBumpTex();
	ID3D10ShaderResourceView* GetEarthAtmos();
	ID3D10ShaderResourceView* GetEarthCloudTex();
	ID3D10ShaderResourceView* GetEarthCityLightTex();
	
};
#endif