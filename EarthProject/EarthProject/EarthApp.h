#ifndef EARTHAPP_H
#define EARTHAPP_H
#include "d3dApp.h"
#include "Light.h"
#include <string>
#include "EarthFx.h"
#include "FullScreenQuad.h"
#include "RenderTexture.h"
#include "RenderToTextureFx.h"
#include "Blur.h"
#include "ViewNormal.h"
#include "GeoSphere.h"
#include "BasicShader.h"
class EarthApp : public D3DApp
{

enum DRAWPLAN
{
	PLAIN,
	JBLUR,
	BLOOM,
	VIEWNORM,
	GEO

};
DRAWPLAN Drawing;


public:
	EarthApp(HINSTANCE hInstance);
	~EarthApp();

	void initApp();

	void updateScene(float dt);
	void drawScene();
	void onResize();
	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	GeoSphere* geo;
	Model* geoModel;
private:
	void BuildFX();
	void BuildVertexLayout();
	
	void PlainEarth();
	void BlurEarth();
	void BloomEarth();
	void ViewNorms();
	void GeoSphereDraw();
private:
	POINT mOldMousePos;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;
	D3DXMATRIX mOrtho;
	D3DXVECTOR3 mEyePos;

	ViewNormal* normalM;
	ViewNormal* geoNormal;
	
	EarthFx earthFx;

	FullScreenQuad* quad;
	RenderTexture* renderTex;
	RenderTexture* renderTex2;
	RenderTexture* renderTex3;
	RenderToTextureFx* renderTexFx;
	
	BlurFx* blurFx;
	BlurFx* blurFx2;
	BasicShader* shader;
};


#endif 