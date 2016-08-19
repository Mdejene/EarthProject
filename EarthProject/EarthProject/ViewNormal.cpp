#include "ViewNormal.h"

ViewNormal::ViewNormal()
{
	md3dDevice = 0;
	mVB = 0;
	mIB = 0;
	mVerts = 0;
	mNumV = 0;
	
}

ViewNormal::~ViewNormal()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	
	
}

void ViewNormal::init(ID3D10Device* device, std::vector<VertexBuffer1> vertex, int size)
{
	md3dDevice = device;
	//model.init(device, EarthM, 0.55f, D3DXVECTOR3(0, 0, 0), "n");

	mNumV = (size/3)*2;
	mVerts = new Vertex2[mNumV];
	
	DWORD* ind = new DWORD[mNumV];
	int c = 0;
	int d = 0;
	D3DXVECTOR3 averagePos;
	D3DXVECTOR3 averageNorm;
	for(int i = 0; i < size/3; i++)
	{
		averagePos = D3DXVECTOR3(0, 0, 0);
		averageNorm = D3DXVECTOR3(0, 0, 0);
		for(int b = 0; b <3; b++)
		{
			averagePos +=  vertex[c].pos;
			averageNorm +=  vertex[c].norm;	
			
			c++;			
		}
		D3DXVECTOR3 extrudedNorm = D3DXVECTOR3(0, 0, 0);
		averageNorm /=3;
		averagePos /= 3;
		extrudedNorm = averageNorm;
		mVerts[d].pos = D3DXVECTOR3(averagePos);
		mVerts[d].texCoord = D3DXVECTOR2(0, 0);
		d++;
		mVerts[d].pos = D3DXVECTOR3(averagePos.x + extrudedNorm.x, averagePos.y + extrudedNorm.y, averagePos.z + extrudedNorm.z);
		mVerts[d].texCoord = D3DXVECTOR2(1, 1);
		d++;		
	}

	for(int i = 0; i < mNumV; i++)
	{
		ind[i] = i;
	}

	int i = 0;


	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex2) * mNumV;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mVerts[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumV;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;

	
    D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem =  &ind[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));

	delete ind;
	ind=0;
	delete mVerts;
	mVerts = 0;
}

void ViewNormal::init(ID3D10Device* device, Model* inModel)
{
	md3dDevice = device;
	//model.init(device, EarthM, 0.55f, D3DXVECTOR3(0, 0, 0), "n");

	mNumV = (inModel->GetSize()/3)*2;
	mVerts = new Vertex2[mNumV];
	
	DWORD* ind = new DWORD[mNumV];
	int c = 0;
	int d = 0;
	D3DXVECTOR3 averagePos;
	D3DXVECTOR3 averageNorm;
	for(int i = 0; i < inModel->GetSize()/3; i++)
	{
		averagePos = D3DXVECTOR3(0, 0, 0);
		averageNorm = D3DXVECTOR3(0, 0, 0);
		for(int b = 0; b <3; b++)
		{
			averagePos +=  inModel->model->vertex[c].pos;
			averageNorm +=  inModel->model->vertex[c].normal;	
			
			c++;			
		}
		D3DXVECTOR3 extrudedNorm = D3DXVECTOR3(0, 0, 0);
		averageNorm /=3;
		averagePos /= 3;
		extrudedNorm = averageNorm;
		mVerts[d].pos = D3DXVECTOR3(averagePos);
		mVerts[d].texCoord = D3DXVECTOR2(0, 0);
		d++;
		mVerts[d].pos = D3DXVECTOR3(averagePos.x + extrudedNorm.x, averagePos.y + extrudedNorm.y, averagePos.z + extrudedNorm.z);
		mVerts[d].texCoord = D3DXVECTOR2(1, 1);
		d++;

		
	}

	for(int i = 0; i < mNumV; i++)
	{
		ind[i] = i;
	}

	int i = 0;


	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex2) * mNumV;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mVerts[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumV;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;

	
    D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem =  &ind[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));

	delete ind;
	ind=0;
	delete mVerts;
	mVerts = 0;
}
void ViewNormal::draw()
{
	UINT stride = sizeof(Vertex2);
    UINT offset = 0;
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    md3dDevice->IASetIndexBuffer(mIB,  DXGI_FORMAT_R32_UINT, 0);

}