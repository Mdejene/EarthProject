#include "EarthApp.h"
#include "Camera.h"
#include "Vertex.h"
#include "Model.h"

bool started = false;
bool onOff   = false;
float dtt = 0;
/*! ******************************Direct X EarthApp*************************************
	Display of HLSL Techniques: Blur, Bloom, Lighting, Texturing, View Normals!*/


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

		
	EarthApp theApp(hInstance);
	
	theApp.initApp();

	

	return theApp.run();
}

EarthApp::EarthApp(HINSTANCE hInstance) : D3DApp(hInstance)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP);
	D3DXMatrixIdentity(&mOrtho);

	
	quad = 0;
	renderTex = 0;
	renderTex2 = 0;
	renderTexFx = 0;
	blurFx = 0;
	normalM = 0;
}

EarthApp::~EarthApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	
	delete quad;
	delete renderTex;
	delete renderTex2;
	delete renderTexFx;
	delete blurFx;
	delete normalM;

	normalM = 0;
	blurFx = 0;
	quad = 0;
	renderTex = 0;
	renderTex2 = 0;
	renderTexFx = 0;
	
}

void EarthApp::initApp()
{
	D3DApp::initApp();
	mView = GetCamera().view();
	mProj = GetCamera().proj();

	earthFx.InitShader(md3dDevice);
	
	Drawing = PLAIN;
	
	quad = new FullScreenQuad;
	quad->Init(md3dDevice, mClientWidth, mClientHeight);

	renderTex = new RenderTexture;
	renderTex->SetUp(md3dDevice, mClientWidth, mClientHeight, 0.1f, 1000);

	renderTex2 = new RenderTexture;
	renderTex2->SetUp(md3dDevice, mClientWidth, mClientHeight, 0.1f, 1000);
	

	renderTexFx = new RenderToTextureFx;
	renderTexFx->InitShader(md3dDevice);
	
	blurFx = new BlurFx();
	blurFx->InitShader(md3dDevice, 3, 1, (float)mClientWidth, (float)mClientHeight);

	Model m;
	m.init(md3dDevice, EarthM, 0.51f, D3DXVECTOR3(0, 0, 0), "NONA");
	Vertex* v= new Vertex[earthFx.GetEarth()->GetEarthModel()->GetSize()];
	
	v = earthFx.GetEarth()->GetEarthModel()->GetVertexList();
	normalM = new ViewNormal();
	normalM->init(md3dDevice, earthFx.GetEarth()->GetEarthModel());
	geoNormal = new ViewNormal();
	//geo = new GeoSphere();
	//geo->CreateGeoSphere(4);
	geoModel = new Model();
	geoModel->init(md3dDevice, geo);
	geoNormal->init(md3dDevice, geoModel->geo->Buffer, geoModel->geo->Inds());
	shader = new BasicShader();
	shader->InitFx(md3dDevice);
	
	int i = 0;
}

void EarthApp::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	GetCamera().setLens(0.25f*PI, aspect, 1.0f, 1000000.0f);
	
	if(started)
	{
		quad->Init(md3dDevice, mClientWidth, mClientHeight);
		renderTex->SetUp(md3dDevice, mClientWidth, mClientHeight, 0.1f, 1000);
		renderTex2->SetUp(md3dDevice, mClientWidth, mClientHeight, 0.1f, 1000);
		blurFx->InitShader(md3dDevice, 3, 2, (float)mClientWidth, (float)mClientHeight);
	}
	
}

void EarthApp::updateScene(float dt)
{
	D3DApp::updateScene(dt);
	
	mView = GetCamera().view();
	mProj = GetCamera().proj();
	mOrtho = GetCamera().ortho();
	dtt += dt;
	if(GetAsyncKeyState('A') & 0x8000)	GetCamera().strafe(-40*dt);
	if(GetAsyncKeyState('D') & 0x8000)	GetCamera().strafe(+40*dt);
	if(GetAsyncKeyState('W') & 0x8000)	GetCamera().walk(+40*dt);
	if(GetAsyncKeyState('S') & 0x8000)	GetCamera().walk(-40*dt);
	if(GetAsyncKeyState('1') & 0x8000)	Drawing = PLAIN;
	if(GetAsyncKeyState('2') & 0x8000)	Drawing = JBLUR;
	if(GetAsyncKeyState('3') & 0x8000)	Drawing = BLOOM;
	if(GetAsyncKeyState('4') & 0x8000)	Drawing = VIEWNORM;
	if(GetAsyncKeyState('5') & 0x8000)	Drawing = GEO;
	GetCamera().rebuildView();

	earthFx.GetEarth()->Update();
}


void EarthApp::drawScene()
{
	started = true;
	std::wstring mDraw;
	md3dDevice->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	switch(Drawing)
	{
	case PLAIN:
		PlainEarth();
		mDraw =  L"Drawing: PLAIN" ;
		break;	
	case JBLUR:
		BlurEarth();
		mDraw =  L"Drawing: BLUR" ;
		break;
	case BLOOM:
		BloomEarth();
		mDraw =  L"Drawing: BLOOM" ;
		break;
	case VIEWNORM:
		ViewNorms();
		mDraw =  L"Drawing: VIEWNORM" ;
		break;
	case GEO:
		GeoSphereDraw();
		mDraw =  L"Drawing: GeoSphere" ;
		break;


	}
	



	RECT R = {5, 20, 0, 0};	
	RECT R2 = {550, 20, 0, 0};	
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, WHITE);
	mFont->DrawText(0, mDraw.c_str(), -1, &R2, DT_NOCLIP, RED);
	mSwapChain->Present(0, 0);
	
}




LRESULT EarthApp::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	POINT mousePos;
	int dx = 0;
	int dy = 0;
	switch(msg)
	{
	case WM_LBUTTONDOWN:
		if( wParam & MK_LBUTTON )
		{
			SetCapture(mhMainWnd);

			mOldMousePos.x = LOWORD(lParam);
			mOldMousePos.y = HIWORD(lParam);
		}
		return 0;

	case WM_LBUTTONUP:
		ReleaseCapture();
		return 0;

	case WM_MOUSEMOVE:
		if( wParam & MK_LBUTTON )
		{
			mousePos.x = (int)LOWORD(lParam); 
			mousePos.y = (int)HIWORD(lParam); 

			dx = mousePos.x - mOldMousePos.x;
			dy = mousePos.y - mOldMousePos.y;

			GetCamera().pitch( dy * 0.0087266f );
			GetCamera().rotateY( dx * 0.0087266f );
			
			mOldMousePos = mousePos;
		}
		return 0;
	}

	return D3DApp::msgProc(msg, wParam, lParam);
}
 


/*! FUNCTION PLAIN EARTH: 
Renders Regular earth
!*/
void  EarthApp::PlainEarth()
{
	D3DApp::drawScene();
		earthFx.Render(dtt);
}
/*! FUNCTION BlurEarth: 
-Renders to offscreen render target
- Grabs the texture to blur it on a second  orhtro rendertarget
-Renders to screen on full screen quad
!*/
void  EarthApp::BlurEarth()
{
	D3DXMATRIX world;
	D3DXMATRIX ortho;
	D3DXMATRIX proj;

	D3DXMatrixIdentity(&world);	
	
	D3DXMATRIX  viewq;
	D3DXMatrixIdentity(&viewq);
	D3DXMatrixLookAtLH(&viewq, &D3DXVECTOR3(0, 0,-1), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0,1 ,0));
		
	D3DApp::drawScene();	

	renderTex->SetRenderTarget();
	renderTex->ClearRenderTarget();
	
	earthFx.Render(dtt);


	SetBackBufferRenderTarget();	
	resetViewport();
	D3DApp::drawScene();

	ZBuffOff();
	renderTex2->SetRenderTarget();
	renderTex2->ClearRenderTarget();

	renderTex2->GetOrthoMatrix(ortho);
	quad->draw();
	blurFx->SetShaderVars(renderTex->GetShaderResourceView(), ortho);
	blurFx->SetTech("JustBlur");
	blurFx->Render(NUM_VERTEX);
	
	ZBuffOn();
	SetBackBufferRenderTarget();	
	resetViewport();
	D3DApp::drawScene();
	

	ZBuffOff();
	quad->draw();	
	
	renderTexFx->SetShaderVars(renderTex2->GetShaderResourceView(), ortho, world, viewq);
	renderTexFx->Render(NUM_VERTEX, 0, 0);	
}
/*! FUNCTION BloomEarth: 
-Renders to offscreen render target
- Grabs the texture to blur it on a second  orhtro rendertarget
- Adds blured tex to original tex to create bloom
-Renders to screen on full screen quad
!*/
void  EarthApp::BloomEarth()
{
	D3DXMATRIX world;
	D3DXMATRIX ortho;
	D3DXMATRIX proj;

	D3DXMatrixIdentity(&world);	
	
	D3DXMATRIX  viewq;
	D3DXMatrixIdentity(&viewq);
	D3DXMatrixLookAtLH(&viewq, &D3DXVECTOR3(0, 0,-1), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0,1 ,0));
		
	D3DApp::drawScene();	

	renderTex->SetRenderTarget();
	renderTex->ClearRenderTarget();
	
	earthFx.Render(dtt);


	SetBackBufferRenderTarget();	
	resetViewport();
	D3DApp::drawScene();

	ZBuffOff();
	renderTex2->SetRenderTarget();
	renderTex2->ClearRenderTarget();

	renderTex2->GetOrthoMatrix(ortho);
	quad->draw();
	blurFx->SetShaderVars(renderTex->GetShaderResourceView(), ortho);
	blurFx->Render(NUM_VERTEX);
	
	ZBuffOn();
	SetBackBufferRenderTarget();	
	resetViewport();
	D3DApp::drawScene();
	

	ZBuffOff();
	quad->draw();	
	
	renderTexFx->SetShaderVars(renderTex2->GetShaderResourceView(), ortho, world, viewq);
	renderTexFx->Render(NUM_VERTEX, 0, 0);	
	
}
/*! FUNCTION ViewNorms: 
-Renders to offscreen render target the earth plus norm verts
- Grabs the texture to blur it on a second  orhtro rendertarget
-Renders to screen on full screen quad
!*/
void  EarthApp::ViewNorms()
{
	D3DXMATRIX world;
	D3DXMATRIX ortho;
	D3DXMATRIX proj;
	D3DXMatrixIdentity(&world);
	started = true;
	
	D3DXMATRIX  viewq;
	D3DXMatrixIdentity(&viewq);
	D3DXMatrixLookAtLH(&viewq, &D3DXVECTOR3(0, 0,-1), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0,1 ,0));

	
	D3DApp::drawScene();	

	renderTex->SetRenderTarget();
	renderTex->ClearRenderTarget();
	
	earthFx.Render(dtt);

	normalM->draw();
	
	int c = 0;
	md3dDevice->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	renderTexFx->SetShaderVars(renderTex2->GetShaderResourceView(), GetCamera().proj(),  *earthFx.GetEarth()->GetEarthModel()->WorldMat(), GetCamera().view());
	renderTexFx->SetTechnique("NormalView");
	
		
	renderTexFx->Render(normalM->GetNumFaces(), 0, 0);
	
	

	SetBackBufferRenderTarget();	
	D3DApp::drawScene();

	ZBuffOff();
	renderTex2->SetRenderTarget();
	renderTex2->ClearRenderTarget();

	renderTex2->GetOrthoMatrix(ortho);
	quad->draw();
	blurFx->SetShaderVars(renderTex->GetShaderResourceView(), ortho);
	blurFx->Render(NUM_VERTEX);
	
	ZBuffOn();
	SetBackBufferRenderTarget();	
	resetViewport();
	D3DApp::drawScene();
	

	ZBuffOff();
	quad->draw();	
	
	renderTexFx->SetShaderVars(renderTex2->GetShaderResourceView(), ortho, world, viewq);
	renderTexFx->Render(NUM_VERTEX, 0, 0);
	
	
}

void EarthApp::GeoSphereDraw()
{	D3DApp::drawScene();
	md3dDevice->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3DXMATRIX world;
	D3DXMATRIX view = GetCamera().view();
	D3DXMATRIX proj = GetCamera().proj();
	D3DXMatrixIdentity(&world);
	//D3DXMatrixScaling(&world, 100.0f, 100.0f, 100.0f);
	D3DXMATRIX rot;

	D3DXMatrixRotationY(&world, dtt);
	shader->SetShaderVars(proj, world,view, earthFx.mEarth.GetEarthTex());	
	
	geoModel->draw();
	shader->Render(geoModel->mNumFaces);

	//geoNormal->draw();
	//D3DXMATRIX x;
	//D3DXMatrixIdentity(&x);
	//md3dDevice->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	//renderTexFx->SetShaderVars(0, GetCamera().proj(),  x, GetCamera().view());
	//renderTexFx->SetTechnique("NormalView");
	//
	//	
	//renderTexFx->Render(normalM->GetNumFaces(), 0, 0);
	
	
}