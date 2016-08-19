#ifndef VERTEX_H
#define VERTEX_H

#include "d3dUtil.h"

struct Vertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXCOLOR   diffuse;
	D3DXCOLOR   spec;
	D3DXVECTOR4 texCoord;
};

struct Vertex2
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 texCoord;
};

struct VertexBuffer
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 col;
	D3DXVECTOR3 col2;
	D3DXVECTOR3 texC;
};

struct VertexBuffer1
{
	D3DXVECTOR3 pos;	
	D3DXVECTOR2 texCoord;	
	D3DXVECTOR3 norm;
};
#endif // VERTEX_H