
#ifndef MODEL_H
#define MODEL_H
#include "MLoader.h"
#include "d3dUtil.h"
#include "cVec.h"

#include "GeoSphere.h"
//***********************************************MODEL CLASS***************************************************
//Model class the impelents the data from my MLoader class tha includes all the verts indices normals tex coord

class Model
{
public:

	Model();
	~Model();
	GeoSphere* geo;
	void init(ID3D10Device* device,  GeoSphere*);
	void init(ID3D10Device* device, ModelType type, float scale, D3DXVECTOR3 Pivot, bool ifAtmos);
	void draw(bool ifAtmos);
	void draw();
	int mSize;
	D3DXMATRIX* WorldMat();

	void SetWorldMat( D3DXMATRIX );
	int GetSize() { return mSize; };
	char* Technique() {return  &tech[0]; }
	Vertex* GetVertexList();
	DWORD mNumFaces;
	MLoader* model;
private:	

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	ID3D10Buffer* mVBd;
	ID3D10Buffer* mIBd;
	D3DXMATRIX world;	
	
	
	char* tech;

};


#endif
