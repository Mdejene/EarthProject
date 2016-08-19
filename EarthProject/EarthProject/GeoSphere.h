#ifndef GEOSPHERE_H
#define GEOSPHERE_H
#include <vector>
#include <math.h>
#include <map>
#include "Vertex.h"
#include <math.h>
#include "cVec.h"
class Model;
#define POINTS 12
#define VERTEXS 3
#define K (double)((1.0 + sqrt(5.0)) /2.0) // const variable for the icosh
const double Z = (double)0.8506508083520400 ;//Tao
const double X = (double)0.5257311121191336 ;//one

static float VERS[POINTS][VERTEXS] =
		{
			{-X,0.0,Z},{X,0.0,Z},{-X,0.0,-Z},{X,0.0,-Z},
			{0.0,Z,X},{0.0,Z,-X},{0.0,-Z,X},{0.0,-Z,-X},
			{Z,X,0.0},{-Z,X,0.0},{Z,-X,0.0},{-Z,-X,0.0}
		};

			
		
static UINT INDS[20][3] = {
  {1,4,0},{4,9,0},{4,5,9},{8,5,4},{1,8,4},
  {1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
  {3,10,7},{10,6,7},{6,11,7},{6,0,11},{6,1,0},
  {10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7}};

class GeoSphere
{
public:
	GeoSphere();
	GeoSphere(const GeoSphere& other);
	~GeoSphere();
	void  CreateGeoSphere(int lod);
	std::vector<VertexBuffer1> Buffer;
	std::vector<UINT> Indice;
private:
	int AddVertex( D3DXVECTOR3);
	int MidPoint(int x, int y);

	
	
	
	//const static double Icosahedron[POINTS][VERTEXS];	

	void CalculateFinalVertex();
	void GetSphericalCoordinates(double r, double& theta, double& phi);
	Model* model;

	
private:
	int index;
	int _iCount;
	int _vCount;
	double _lastX;
	double _lastY;
public:
	
	bool in;
	Model* GetModel() {return model;}
	int Inds() const {return _iCount;}
	int Verts() const {return _vCount;}
	DWORD* Faces() {return FinIndexs.data();}
	D3DXVECTOR3* Face() {return &Indices[0];}
	D3DXVECTOR3* Vector() {return &Vectors[0];}
	std::vector<D3DXVECTOR3> norms;
	std::vector<DWORD> FinIndexs;
	std::vector<D3DXVECTOR2> tex;
	std::map<long long, int> Cache;
	std::vector<D3DXVECTOR3> Vectors;
	std::vector<D3DXVECTOR3> Indices;
	std::vector<UINT> oInds;;
	std::vector<Vertex> FinalVertexs;
	static Vec3F Normalize(Vec3F in) 
	{ 
		double length = sqrt((in.x * in.x) + (in.y * in.y) + (in.z * in.z)) ;
		return Vec3F(in.x/length, in.y/length, in.z/length);
	};

	
	
	
};
#endif 
		