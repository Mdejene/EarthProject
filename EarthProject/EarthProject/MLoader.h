#ifndef MLOADER_H
#define MLOADER_H

#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "d3dUtil.h"
#include "Vertex.h"
using namespace std;




enum ModelType
{
	StarM,
	TeaPotM,
	TrashBinM,
	ModelBoxM,
	TerrainM,
	HandM,
	SunM,
	MoonM,
	AsteroidM,
	EarthM,
	PlaneM
};

class MLoader
{

private:	

	vector<D3DXVECTOR3> vertices;
	
	vector<D3DXVECTOR3> normals;
	vector<D3DXVECTOR3> texCoord;
	vector<FLOAT>nNormal;

	void LoadObj(ModelType type, float Scale, D3DXVECTOR3 offset);

	int numVerts;
	int numVFaces;

	int numNormal;
	int numNFaces;

	int numTexCoord;
	int numTFaces;


	int Scale;

	void CalNormal();
public:

	MLoader(ModelType type, float Scale, D3DXVECTOR3 offset);
	~MLoader();	
	vector<DWORD> vfaces;
	vector<DWORD> tfaces;
	vector<DWORD> nfaces;
	vector<Vertex> vertex;
	DWORD* Faces() { return &vfaces[0]; }
	//DWORD* nFaces() { return &nfaces[0]; }

	D3DXVECTOR3* Verts() { return &vertices[0]; }
	Vertex* Vertexs() {return &vertex[0]; }

	D3DXVECTOR3* Normals() {return &normals[0];}

	const int NumVerts() const {return numVerts; }
	int NumFaces() const {return numVFaces; }

	
	
	
};

#endif