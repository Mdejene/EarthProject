#ifndef VIEWNORMAL_H
#define VIEWNORMAL_H
#include "Model.h"
#include "Vertex.h"
class ViewNormal
{
private:
	
	int mNumV;
	Vertex2* mVerts;
public:
	ViewNormal();
	~ViewNormal();

	void draw();
	void init(ID3D10Device* device, std::vector<VertexBuffer1> vertex, int size);
	void init(ID3D10Device* device, Model* model);
	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	int GetNumFaces(){return mNumV;}

};
#endif