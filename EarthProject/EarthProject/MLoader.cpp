#include "MLoader.h"
#include <string>

//*********************************************MLOADER CLASS************************************************
//Reads data line by line from the obj file as text and inputs the info into vectors for the model class the use

D3DXVECTOR3 CrossProduct();

MLoader::MLoader(ModelType type, float Scale, D3DXVECTOR3 offset) : numVerts(0), numVFaces(0), numNormal(0), numNFaces(0), numTFaces(0)
{
	LoadObj(type, Scale, offset);
	
}

MLoader::~MLoader()
{

}

void MLoader::LoadObj(ModelType type, float Scale, D3DXVECTOR3 offset)
{
	char* fileName;

	switch(type)
	{
	case StarM:
		fileName = "star.obj";
			break;
	case TeaPotM:
		fileName = "f1.obj";
		break;
	case TrashBinM:
		fileName = "TrashBin.obj";
		break;
	case ModelBoxM:
		fileName = "box.obj";
		break;
	case TerrainM:
		fileName = "Models/asd.obj";
		break;
	case HandM:
		fileName = "earth.obj";
		break;
	case EarthM:
		fileName = "Models/sphere.obj";
		break;
	case MoonM:
		fileName = "earth.obj";
		break;
	case SunM:
		fileName = "earth.obj";
		break;
	case AsteroidM:
		fileName = "asteroid.obj";


	}
	
	ifstream model(fileName);

	if(!model)
	{
		MessageBoxA(0, "COULD LOAD FILE"  , 0, 0);
	}

	 D3DXVECTOR3 temp;
	 D3DXVECTOR3 temp2;
	 D3DXVECTOR3 temp3;
	 Vertex tempV;

	 string buff;
	 char buffer[255];
	 
	 
	 while(!model.eof())
	 {		 
		 
		 model >> buffer; 
		 
		 if(buffer[0] == 'v')
		 {
			 if(buffer[1] == 'n')
			 {
				 model >> temp.x  >> temp.y >> temp.z; 				
				 temp.z = temp.z * -1;
				 normals.push_back(temp);				 
				 nNormal.push_back(temp.x);
				 nNormal.push_back(temp.y);
				 nNormal.push_back(temp.z);
				 numNormal++;
				 
				 
			 }
			 else if(buffer[1] == 't')
			 {
				 model >> temp.x  >> temp.y >> temp.z; 
				 temp.y =   1- temp.y ;				
				 texCoord.push_back(temp);	
				 numTexCoord ++;
			 }
			 else
			 {
				model >> temp.x  >> temp.y >> temp.z; 
				temp.z *= -1;
				vertices.push_back(temp);
				
				tempV.pos = (temp);						
				model.getline(buffer, 255);
				numVerts++;					
					
			 }

			 
		 }
		 if(buffer[0] == 'f')
		 {
			model.get(buffer[0]);

			model >> temp.x  >> buffer[0] >> temp2.x >> buffer[0] >>temp3.x >> 
					 temp.y  >> buffer[0] >> temp2.y >> buffer[0] >>temp3.y >> 
					 temp.z  >> buffer[0] >> temp2.z >> buffer[0] >>temp3.z; 


			vfaces.push_back((DWORD)temp.z-1);
			vfaces.push_back((DWORD)temp.y-1);
			vfaces.push_back((DWORD)temp.x-1);

			nfaces.push_back((DWORD)temp2.z-1);
			nfaces.push_back((DWORD)temp2.y-1);
			nfaces.push_back((DWORD)temp2.x-1);
			

			tfaces.push_back((DWORD)temp3.z-1);
			tfaces.push_back((DWORD)temp3.y-1);
			tfaces.push_back((DWORD)temp3.x-1);

			numVFaces ++;
			numTFaces ++;
			numNFaces ++;
			
			model.getline(buffer, 255);
		 }

		 if(buffer[0] == '#' || buffer[0] == 'g' )
		 {
			model.getline(buffer, 255);
		 }		
	
	}


	
	 float right = 0;
	 for(DWORD i = 0; i < ((DWORD)numVFaces *3) ; i++)
	{
		
		Vertex v;			
		v.pos = (vertices[ vfaces[i] ] + offset) * Scale;
		v.normal = normals[tfaces[i]]; 
		v.diffuse =  WHITE;
		v.spec = WHITE;
		v.texCoord = D3DXVECTOR4(texCoord[nfaces[i]].x, texCoord[nfaces[i]].y, 1, 1);

		vertex.push_back(v);
		v.pos.x > right? right = v.pos.x : right = right; 
		vfaces[i] = i;
	}	
	int s = 0;
	//CalNormal();		
}

void MLoader::CalNormal()
{

	

	for(DWORD i = 0; i < (UINT)numVFaces ; i++ )
	{
		DWORD i0 = vfaces[i*3+0];
		DWORD i1 = vfaces[i*3+1];
		DWORD i2 = vfaces[i*3+2];

		Vertex v0 = vertex[i0];
		Vertex v1 = vertex[i1];
		Vertex v2 = vertex[i2];

		D3DXVECTOR3 e0 = v1.pos - v0.pos;

		D3DXVECTOR3 e1 = v2.pos - v0.pos;
		D3DXVECTOR3 faceNormal;
		D3DXVec3Cross(&faceNormal, &e0, &e1);

		D3DXVec3Normalize(&faceNormal, &faceNormal);
		
		
		vertex[i0].normal += faceNormal;
		
		vertex[i1].normal += faceNormal;
		vertex[i1].normal += faceNormal;
		

	}

	for(DWORD i = 0; i < (vertex.size()) -1 ; i++)
	{
		D3DXVec3Normalize(&vertex[i].normal, &vertex[i].normal);
		//vertex[i].normal.z = -vertex[i].normal.z;
	}
	int fff = 3;
}


