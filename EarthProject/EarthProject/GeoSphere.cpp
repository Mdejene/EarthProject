#include "GeoSphere.h"

#include "Model.h"
#include "3D Noise.h"


GeoSphere::GeoSphere()
{
	index = 0;
	_iCount = 0;
	_vCount = 0;
	in = false;
	_lastX = 0;
	_lastY = 0;
}

GeoSphere::~GeoSphere()
{
}


void GeoSphere::GetSphericalCoordinates(double r, double& theta, double& phi)
{
	double TWOPI = PI*2;
	double x, y;
	x = theta;
	y = phi;
	//theta =  (atan2(r, phi) /(TWOPI))+0.5 ; 
	//phi =  asin(theta) / (PI)+0.5;
	
	theta  = 0.5 - (atan2(y, r) / TWOPI);
	phi = 0.5 - (2.0* asin(x)/TWOPI);
	
	
	
}

int GeoSphere::AddVertex(D3DXVECTOR3 in)
{	
	double length = sqrt( (in.x * in.x) + (in.y * in.y) + (in.z * in.z) ) ;
   
	D3DXVECTOR3 vec =  D3DXVECTOR3(in.x/length, in.y/length, in.z/length);
	norms.push_back(vec);
	
	Vectors.push_back(vec);
	double phi = 0.0;
	double theta = 0.0;//, x2;
	double xx = vec.x;
	theta = (double)vec.y;
	phi = (double)vec.z;
	GetSphericalCoordinates(xx, theta,phi );
	tex.push_back(D3DXVECTOR2(theta, phi));
	_vCount++;
	return index++;
}

int GeoSphere::MidPoint(int x, int y)
{
	bool leftORright = x < y;
	int vwwv = sizeof(long long);
	long long smallNum = leftORright ? x : y;
	long long bigNum = leftORright ? y : x;
	long long key = (smallNum << 32) + bigNum;

	int keys = -1;
	if(Cache.find(key) != Cache.end())
		keys = Cache.find(key)->second;
	if(keys > -1)
	{
		return keys;
	}

	D3DXVECTOR3 v1 = Vectors[x];
	D3DXVECTOR3 v2 = Vectors[y];	
	D3DXVECTOR3 mid = v1 + v2;
	mid /= 2;
	

	int t = AddVertex(mid);
	
	Cache[key] = t; 
	return t;
}
//Modified from: http://gamedev.stackexchange.com/questions/33631/textureing-subdivided-icosahedron-in-xna-seam-problem

static void RepairTextureWrapSeam(std::vector<VertexBuffer1> &buffer, std::vector<UINT> &indices)
    {
        std::vector<UINT> newIndices;

        int corrections = 0;
       
		std::map<int, int> correctionList; 
		D3DXVECTOR2 lastINdex;
		for (int i = indices.size() - 3; i >= 0; i -= 3)
        {

			D3DXVECTOR3 v0 = D3DXVECTOR3(buffer[indices[i + 0]].texCoord.x, buffer[indices[i + 0]].texCoord.y, 0);
            D3DXVECTOR3 v1 = D3DXVECTOR3(buffer[indices[i + 1]].texCoord.x,  buffer[indices[i + 1]].texCoord.y, 0);
            D3DXVECTOR3 v2 = D3DXVECTOR3(buffer[indices[i + 2]].texCoord.x,  buffer[indices[i + 2]].texCoord.y, 0);
			D3DXVECTOR3 out;
			D3DXVECTOR3 vv1 = (v0 - v1);
			D3DXVECTOR3 vv2 = (v2 - v1);
            D3DXVec3Cross(&out, &vv1, &vv2);
			
            if (out.z <= 0)
            {

                bool corrected = false;

                for (int j = i; j < i + 3; j++)
                {
                    UINT index = indices[j];
					D3DXVECTOR2 texCoord;
                    VertexBuffer1 vertex = buffer[index];
					if (vertex.texCoord.x <= 0.5f ||vertex.texCoord.x >= 0.9f)
                    {
                        /// need to correct this vertex.
						if (correctionList.find(index) != correctionList.end())
							newIndices.push_back(correctionList[index]);
                        else
                        {
							if(vertex.texCoord.x <= 0.5f)
							{
								texCoord = vertex.texCoord;

								texCoord.x   = 1.0f - texCoord.x;
								//texCoord.y   = 1.0f - texCoord.y;
								vertex.texCoord = texCoord;
								corrected = true;
							}
							

							buffer.push_back(vertex);
							UINT correctedVertexIndex = buffer.size()-1;
							correctionList[index] =  correctedVertexIndex;
							newIndices.push_back(correctedVertexIndex);
							
                        }
                    }					
					else
						newIndices.push_back(index);

					
					
                }

                if (corrected)
                    corrections++;
            }
            else
			{
				newIndices.push_back(indices[i]);
				newIndices.push_back(indices[i+1]);
				newIndices.push_back(indices[i+2]);
			}
			lastINdex = D3DXVECTOR2(out.x, out.y);;
        }

       // Debug.WriteLine("Corrected {0} of {1}", corrections, newIndices.Count / 3);


		indices.clear();
        indices = (newIndices);
    }
void GeoSphere::CreateGeoSphere(int lod)
{
	
	float t = (1.0 + sqrt(5.0)) / 2.0;

	AddVertex(D3DXVECTOR3(-1,  t,  0));
	AddVertex(D3DXVECTOR3( 1,  t,  0));
	AddVertex(D3DXVECTOR3(-1, -t,  0));
	AddVertex(D3DXVECTOR3( 1, -t,  0));

	AddVertex(D3DXVECTOR3( 0, -1,  t));
	AddVertex(D3DXVECTOR3( 0,  1,  t));
	AddVertex(D3DXVECTOR3( 0, -1, -t));
	AddVertex(D3DXVECTOR3( 0,  1, -t));

	AddVertex(D3DXVECTOR3( t,  0, -1));
	AddVertex(D3DXVECTOR3( t,  0,  1));
	AddVertex(D3DXVECTOR3(-t,  0, -1));
	AddVertex(D3DXVECTOR3(-t,  0,  1));
	
	// 5 faces around point 0
	Indices.push_back(D3DXVECTOR3(0, 11, 5));
	Indices.push_back(D3DXVECTOR3(0, 5, 1));
	Indices.push_back(D3DXVECTOR3(0, 1, 7));
	Indices.push_back(D3DXVECTOR3(0, 7, 10));
	Indices.push_back(D3DXVECTOR3(0, 10, 11));
	
	// 5 adjacent face
	Indices.push_back(D3DXVECTOR3(1, 5, 9));
	Indices.push_back(D3DXVECTOR3(5, 11, 4));
	Indices.push_back(D3DXVECTOR3(11, 10, 2));
	Indices.push_back(D3DXVECTOR3(10, 7, 6));
	Indices.push_back(D3DXVECTOR3(7, 1, 8));
	
	// 5 faces around t 3
	Indices.push_back(D3DXVECTOR3(3, 9, 4));
	Indices.push_back(D3DXVECTOR3(3, 4, 2));
	Indices.push_back(D3DXVECTOR3(3, 2, 6));
	Indices.push_back(D3DXVECTOR3(3, 6, 8));
	Indices.push_back(D3DXVECTOR3(3, 8, 9));
	
	// 5 adjacent facc
	Indices.push_back(D3DXVECTOR3(4, 9, 5));
	Indices.push_back(D3DXVECTOR3(2, 4, 11));
	Indices.push_back(D3DXVECTOR3(6, 2, 10));
	Indices.push_back(D3DXVECTOR3(8, 6, 7));
	Indices.push_back(D3DXVECTOR3(9, 8, 1));
	_iCount += 20;

	std::vector<D3DXVECTOR3> TempIndices;
	
	for(int i = 0; i < lod; i++)
	{
		int tCon = _iCount;
		TempIndices.clear();
		for(int _in = 0; _in < tCon; _in++)
		{
			D3DXVECTOR3 _ind = Indices[_in];
			int _id1 = MidPoint(_ind.x, _ind.y);
			int _id2 = MidPoint(_ind.y, _ind.z);
			int _id3 = MidPoint(_ind.z, _ind.x);
			 
			TempIndices.push_back(D3DXVECTOR3(_ind.x, _id1, _id3));
			TempIndices.push_back(D3DXVECTOR3(_ind.y, _id2, _id1));
			TempIndices.push_back(D3DXVECTOR3(_ind.z, _id3, _id2));
			TempIndices.push_back(D3DXVECTOR3(_id1  , _id2, _id3));
			
			
		}
		Indices.clear();
		
		Indices = TempIndices;
		this->_iCount = Indices.size();
	}
	this->_iCount = Indices.size();
	//Indice = new UINT[_iCount*3];
	
	for(int i = 0; i < _iCount; i++)
	{
		
		Indice.push_back(Indices.at(i).x);
		Indice.push_back(Indices.at(i).y);
		Indice.push_back(Indices.at(i).z);

	}


	Indices.clear();
	double last = 0.0;
	

	Buffer;// = new VertexBuffer1[_iCount*3];
	for(int i = 0; i < _iCount*3; i++)
	{
		
		

		VertexBuffer1 b;
		D3DXVECTOR3 norm;		
		b.pos = Vectors[Indice[i]];
		double d = Instance().CompleteNoise(	b.pos.x, 	b.pos.y, 	b.pos.z, 7, 2.5, .35);
		D3DXVec3Normalize(&norm, &b.pos);
		D3DXVECTOR3 t = norm * (d);
		b.pos *= 100;
		b.pos += t*3;


		b.texCoord = tex[Indice[i]];
		//b.texCoord.x = 1-b.texCoord.x;
		
		
		
		
		b.norm = D3DXVECTOR3(0, 0, 0);
		Buffer.push_back(b);
		Indice[i] = i;
	}
	
	
	for (int i=0;i< _iCount*3;i+=3)    
	 {
		D3DXVECTOR3 FirstVec = D3DXVECTOR3(0, 0, 0);
        D3DXVECTOR3 SecondVec = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 crossed = D3DXVECTOR3(0, 0, 0);

		for(int u = 0; u < 3; u++)
		{
			FirstVec[u] = Buffer[Indice[i]].pos[u] - Buffer[Indice[i+1]].pos[u];
			SecondVec[u] = Buffer[Indice[i+1]].pos[u] - Buffer[Indice[i+2]].pos[u];
			
			
		}
		D3DXVec3Normalize(&FirstVec, &FirstVec);
		D3DXVec3Normalize(&SecondVec, &SecondVec);
		D3DXVec3Cross(&crossed,&FirstVec, &SecondVec ); 

		
		Buffer[i].norm	= crossed;		
		

    }
	
	RepairTextureWrapSeam(Buffer, Indice);
	_vCount = Buffer.size();
	

	
}





















