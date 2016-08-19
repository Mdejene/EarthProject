#ifndef FULLSCREENQUAD_H
#define FULLSCREENQUAD_H
#include <d3d10.h>
#include <d3dx10.h>
#include "d3dUtil.h"

#define NUM_VERTEX 6
class FullScreenQuad
{
public:
	FullScreenQuad();
	FullScreenQuad(const FullScreenQuad& other);
	~FullScreenQuad();

	void Init(ID3D10Device* device, int windowWidth, int windowHeight);
	void draw();

	

private:
	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVertexBuff;
	ID3D10Buffer*	  mIndexBuff;
	int mNumVertex;
	int mNumIndex;
};
#endif