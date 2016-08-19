#include "FullScreenQuad.h"
#include "Vertex.h"



FullScreenQuad::FullScreenQuad()
{
	mVertexBuff = 0;
	mIndexBuff = 0;
	mNumVertex = 0;
	mNumIndex = 0;
	md3dDevice = 0;

}

FullScreenQuad::FullScreenQuad(const FullScreenQuad& other)
{

}

FullScreenQuad::~FullScreenQuad()
{
	mVertexBuff->Release();
	mIndexBuff->Release();
}

void FullScreenQuad::Init(ID3D10Device* device, int windowWidth, int windowHeight)
{
	md3dDevice = device;

	float left;
	float right;
	float top;
	float bottom;
	mNumVertex = NUM_VERTEX;
	mNumIndex = NUM_VERTEX;

	Vertex2 VertexInfo[NUM_VERTEX];
	DWORD* indexs;
	indexs = new DWORD[NUM_VERTEX];
	
	left	 = (float)(( windowWidth / 2) * -1);	
	right	 = left + (float)windowWidth;
	top		 = (float)(windowHeight / 2);	
	bottom	 = top - (float)windowHeight;

	VertexInfo[0].pos		= D3DXVECTOR3(left, top, 0.0f);
	VertexInfo[0].texCoord  = D3DXVECTOR2(0, 0);

	VertexInfo[1].pos		= D3DXVECTOR3(right, bottom, 0.0f);
	VertexInfo[1].texCoord  = D3DXVECTOR2(1, 1);

	VertexInfo[2].pos		= D3DXVECTOR3(left, bottom, 0.0f);		
	VertexInfo[2].texCoord  = D3DXVECTOR2(0, 1);				  

	VertexInfo[3].pos		= D3DXVECTOR3(left, top, 0.0f);
	VertexInfo[3].texCoord  = D3DXVECTOR2(0, 0);

	VertexInfo[4].pos		= D3DXVECTOR3(right, top, 0.0f);
	VertexInfo[4].texCoord  = D3DXVECTOR2(1, 0);

	VertexInfo[5].pos		= D3DXVECTOR3(right, bottom, 0.0f);
	VertexInfo[5].texCoord  = D3DXVECTOR2(1, 1);
	//1&4------5
	//|  \	   |
	//|   \	   |
	//|	   \   |
	//|	    \  |
	//3-------2&6

	for(DWORD i = 0; i< NUM_VERTEX; i++)
	{
		indexs[i] = i;
	}

	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex2) * NUM_VERTEX;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &VertexInfo[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVertexBuff));

	D3D10_BUFFER_DESC iBuffDesc;
	iBuffDesc.Usage = D3D10_USAGE_IMMUTABLE;
	iBuffDesc.ByteWidth = sizeof(DWORD) * NUM_VERTEX;
	iBuffDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	iBuffDesc.CPUAccessFlags = 0;
	iBuffDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA iData;
	iData.pSysMem = &indexs[0];
	HR(md3dDevice->CreateBuffer(&iBuffDesc, &iData, &mIndexBuff));
	
	delete [] indexs;
	indexs = 0;
	
}

void FullScreenQuad::draw()
{
	UINT stride = sizeof(Vertex2);
    UINT offset = 0;
    md3dDevice->IASetVertexBuffers(0, 1, &mVertexBuff, &stride, &offset);
    md3dDevice->IASetIndexBuffer(mIndexBuff,  DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

