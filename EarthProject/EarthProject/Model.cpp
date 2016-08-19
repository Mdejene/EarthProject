#include "Model.h"
#include "MLoader.h"


Model::Model() : mNumFaces(0), md3dDevice(0), mVB(0), mIB(0),mVBd(0), mIBd(0)
{
	D3DXMatrixIdentity(&world);
	
}

Model::~Model()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	delete model;
	model = 0;
}

D3DXMATRIX* Model::WorldMat() 
{
	return &world;
}

void Model::init(ID3D10Device* device,  GeoSphere* geo1)
{
	geo = new GeoSphere;
	geo->CreateGeoSphere(8);

	md3dDevice = device;
	
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(VertexBuffer1) * geo->Verts();;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &geo->Buffer[0];
	vinitData.SysMemPitch = 0;
	vinitData.SysMemSlicePitch = 0;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVBd));

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_DEFAULT;	
	ibd.ByteWidth = sizeof(UINT) * geo->Inds()*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
	std::vector<DWORD> faceee;

	/*for(int i = 0; i < geo->Inds()*3; i++)
	{
		faceee.push_back(]);
	}*/
    D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &geo->Indice[0];
	iinitData.SysMemPitch = 0;
	iinitData.SysMemSlicePitch = 0;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIBd));
	mNumFaces =  geo->Inds()*3;
	this->mSize = mNumFaces /3;
	//delete geo;
}

void Model::init(ID3D10Device* device, ModelType type, float scale, D3DXVECTOR3 Pivot, bool ifAtmos)
{
	model = new MLoader(type, scale, Pivot);
	mNumFaces = model->NumFaces() *3;
	md3dDevice = device;
	
	if(ifAtmos == true)
	{
		D3D10_BUFFER_DESC vbd;
		vbd.Usage = D3D10_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(VertexBuffer) * mNumFaces;
		vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		
		VertexBuffer* vb = new VertexBuffer[35640];
		for(int i = 0; i <35640; i++ )
		{
			vb[i].pos = model->vertex[i].pos;
			vb[i].col = D3DXVECTOR3(0, 0, 0);
			vb[i].col2 = D3DXVECTOR3(0, 0, 0);
			vb[i].texC =  D3DXVECTOR3(0, 0, 0);
			if(i > 35638)
				bool a = true;

		}
		D3D10_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vb;
		HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	}
	else
	{
		D3D10_BUFFER_DESC vbd;
		vbd.Usage = D3D10_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * mNumFaces;
		vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;

		D3D10_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &model->vertex[0];
		HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
	}
	

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;

	
    D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem =  &model->vfaces[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	mSize = mNumFaces;

}

void Model::draw(bool ifAtmos)
{
	UINT stride;
	if(ifAtmos == true) 
		stride = sizeof(VertexBuffer);
	else 
		stride = sizeof(Vertex);

    UINT offset = 0;
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    md3dDevice->IASetIndexBuffer(mIB,  DXGI_FORMAT_R32_UINT, 0);
    md3dDevice->DrawIndexed(mNumFaces , 0, 0);

}

void Model::draw()
{
	UINT stride;
	UINT offset = 0;
	stride =sizeof(VertexBuffer1);
    md3dDevice->IASetVertexBuffers(0, 1, &mVBd, &stride, &offset);
    md3dDevice->IASetIndexBuffer(mIBd,  DXGI_FORMAT_R32_UINT, 0);
	
}
void Model::SetWorldMat(D3DXMATRIX Matrix)
{
	world = Matrix;
}

Vertex* Model::GetVertexList() 
{
	return &model->vertex[0];
}