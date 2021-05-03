#ifndef _dxrr
#define _dxrr
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "TerrenoRR.h"
#include "Camara.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "ModeloRR.h"
#include "Modelo001church.h"
#include "M02bull.h"
#include "M03houseEastern.h"
#include "M04woodenCabinA.h"
#include "M04woodenCabinB.h"
#include "M05RuinedBuilding.h"
#include "M06campfire.h"
#include "M07tree.h"
#include "M09molino.h"
#include "M10flag.h"
#include "M11barrel.h"
#include "M12bride.h"
#include "M13pozo.h"
#include "M14TreeMonster.h"
#include "M15minibridge.h"
#include "M16fountainA.h"
#include "M16fountainB.h"
#include "M17castleA.h"
#include "M17castleB.h"
#include "M18MarketGray.h"
#include "M19Rock.h"

#include "XACT3Util.h"

class DXRR{	

private:
	int ancho;
	int alto;
public:	
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState *alphaBlendState, *commonBlendState;

	int frameBillboard;

	TerrenoRR *terreno;
	SkyDome *skydome;
	BillboardRR *billboard;
	Camara *camara;
	ModeloRR* model;
	Modelo001church* m01church;
	M02bull* m02bull;
	M02bull* m02bull02;
	ModeloRR* m02bull03;
	ModeloRR* m02bull04;
	M03houseEastern* m03houseEastern;
	M04woodenCabinA* m04woodenCabinA;
	M04woodenCabinB* m04woodenCabinB;
	M05RuinedBuilding* m05RuinedBuilding;
	M06campfire* m06campfire;
	M07tree* m07tree;
	ModeloRR* m08;

	M09molino* m09molino;
	ModeloRR* m09molinoE1;
	ModeloRR* m09molinoE2;
	ModeloRR* m09molinoE3;

	M10flag* m10flag;
	M11barrel* m11barrel;
	M12bride* m12bride;
	M13pozo* m13pozo;
	M14TreeMonster* m14TreeMonster;
	M15minibridge* m15minibridge;
	M16fountainA* m16fountainA;
	M16fountainB* m16fountainB;
	M17castleA* m17castleA;
	M17castleB* m17castleB;
	M18MarketGray* m18MarketGray;
	M19Rock* m19Rock;
	
	float izqder;
	float arriba;
	float derech;
	float abajo;
	float vel;
	bool breakpoint;
	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];

	XACTINDEX cueIndex;
	CXACT3Util m_XACT3;
	
    DXRR(HWND hWnd, int Ancho, int Alto)
	{
		breakpoint = false;
		frameBillboard = 0;
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		IniciaD3D(hWnd);
		izqder = 0;
		arriba = 0;
		abajo = 0;
		derech = 0;
		billCargaFuego();
		camara = new Camara(D3DXVECTOR3(0,80,6), D3DXVECTOR3(0,80,0), D3DXVECTOR3(0,1,0), Ancho, Alto);
		terreno = new TerrenoRR(300, 300, d3dDevice, d3dContext);
		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"SkyDome2.png");
		billboard = new BillboardRR(L"Assets/Billboards/fuego-anim.png",L"Assets/Billboards/fuego-anim-normal.png", d3dDevice, d3dContext, 5);
		model = new ModeloRR(d3dDevice, d3dContext, "Assets/Cofre/Cofre.obj", L"Assets/Cofre/Cofre-color.png", L"Assets/Cofre/Cofre-spec.png", 0, 0);
		
		m01church = new Modelo001church(d3dDevice, d3dContext, "Assets//001_Church_01//001_ChurchNew.obj", L"Assets/001_Church_01/chapel_diffuse.jpg", L"Assets/Cofre/Cofre-spec.png", -85.0f, 41.0f);
		m02bull = new M02bull(d3dDevice, d3dContext, "Assets//002_Bull_3d//002_BullNew01.obj", L"Assets/002_Bull_3d/Bulltexture2.png", L"Assets/Cofre/Cofre-spec.png", -10.0f, -129.0f);
		m03houseEastern = new M03houseEastern(d3dDevice, d3dContext, "Assets//003_Middle_Eastern_Houses_OBJ//003_EasternHouseNew.obj", L"Assets/003_Middle_Eastern_Houses_OBJ/Bulding8Paint.jpg", L"Assets/Cofre/Cofre-spec.png", -10.0f, -129.0f);
		m04woodenCabinA = new M04woodenCabinA(d3dDevice, d3dContext, "Assets//004_WoodenCabinObj//004_WoodenC01New.obj", L"Assets/004_WoodenCabinObj/WoodCabinDif.jpg", L"Assets/Cofre/Cofre-spec.png", -86.0f, -23.0f);
		m04woodenCabinB = new M04woodenCabinB(d3dDevice, d3dContext, "Assets//004_WoodenCabinObj//004_WoodenC02New.obj", L"Assets/004_WoodenCabinObj/WoodCabinDif.jpg", L"Assets/Cofre/Cofre-spec.png", -86.0f, -23.0f);
		m05RuinedBuilding = new M05RuinedBuilding(d3dDevice, d3dContext, "Assets//005_Ruined_building_OBJ//005_Newruinedbuilding.obj", L"Assets/005_Ruined_building_OBJ/text.jpg", L"Assets/Cofre/Cofre-spec.png", -50.0f, 44.0f);
		m06campfire = new M06campfire(d3dDevice, d3dContext, "Assets//006_campfire//006_CampfireNew.obj", L"Assets/006_campfire/brick.jpg", L"Assets/Cofre/Cofre-spec.png", 22.0f, 80.0f);
		m07tree = new M07tree(d3dDevice, d3dContext, "Assets//007_Tree_OBJ//007_TreeNew.obj", L"Assets/007_Tree_OBJ/Tree.jpg", L"Assets/Cofre/Cofre-spec.png", -10.0f, -92.0f);
		m09molino = new M09molino(d3dDevice, d3dContext, "Assets//009Molino//MolinoNew.obj", L"Assets/009Molino/windmill_diffuse.png", L"Assets/Cofre/Cofre-spec.png", -120.0f, 0.0f);
		m10flag = new M10flag(d3dDevice, d3dContext, "Assets//010_flag//0010_FlagNew.obj", L"Assets//010_flag/010_FlagB.jpg", L"Assets/Cofre/Cofre-spec.png", 63.0f, -12.0f);
		m11barrel = new M11barrel(d3dDevice, d3dContext, "Assets//011_barrel//0011_BarrelNew.obj", L"Assets/011_barrel/barrel_diffuse.png", L"Assets/Cofre/Cofre-spec.png", -23.5f, -6.0f);
		m12bride = new M12bride(d3dDevice, d3dContext, "Assets//012_Bridge//0012_BridgeNew.obj", L"Assets/012_Bridge/BridgeMap.jpg", L"Assets/Cofre/Cofre-spec.png", -153.0f, -100.0f);
		m13pozo = new M13pozo(d3dDevice, d3dContext, "Assets//013_Pozo//PozoNew.obj", L"Assets/013_Pozo/brick.jpg", L"Assets/Cofre/Cofre-spec.png", -20.0f, -36.0f);
		m14TreeMonster = new M14TreeMonster(d3dDevice, d3dContext, "Assets//014_treem//0014_TreeNew.obj", L"Assets/014_treem/014color.png", L"Assets/Cofre/Cofre-spec.png", 10.0f, 10.0f);
		m15minibridge = new M15minibridge(d3dDevice, d3dContext, "Assets//015_MiniBridge//0015_BridgeNew.obj", L"Assets/015_MiniBridge/Castlewalls_1.png", L"Assets/Cofre/Cofre-spec.png", 37.0f, -105.0f);
		m16fountainA = new M16fountainA(d3dDevice, d3dContext, "Assets//016_Fuente//0016_Bountain01NewNew.obj", L"Assets/016_Fuente/germany010.jpg", L"Assets/Cofre/Cofre-spec.png", 10.0f, 10.0f);
		m16fountainB = new M16fountainB(d3dDevice, d3dContext, "Assets//016_Fuente//0016_Bountain02New.obj", L"Assets/016_Fuente/seawaterfull2.jpg", L"Assets/Cofre/Cofre-spec.png", 10.0f, 10.0f);
		
		m17castleA = new M17castleA(d3dDevice, d3dContext, "Assets//017_Castelo//017_CastleNewA01.obj", L"Assets/017_Castelo/download.jpg", L"Assets/Cofre/Cofre-spec.png", 100.0f, -25.0f);
		m17castleB = new M17castleB(d3dDevice, d3dContext, "Assets//017_Castelo//017_CastleNewA02.obj", L"Assets/017_Castelo/download2.jpg", L"Assets/Cofre/Cofre-spec.png", 100.0f, -25.0f);
		m18MarketGray = new M18MarketGray(d3dDevice, d3dContext, "Assets//018_Market//MarketNewNew.obj", L"Assets/018_Market/Market_Texture.png", L"Assets/Cofre/Cofre-spec.png", 9.0f, 45.0f);
		m19Rock = new M19Rock(d3dDevice, d3dContext, "Assets//019_Rock//RockNew.obj", L"Assets/019_Rock/Rock_1_Base_Color.jpg", L"Assets/Cofre/Cofre-spec.png", 90.0f, 133.0f);
		

	}

	~DXRR()
	{
		LiberaD3D();
		m_XACT3.Terminate();
	}
	
	bool IniciaD3D(HWND hWnd)
	{
		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[]=
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[]=
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for(driver = 0; driver<totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels, 
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if(SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if(FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}
		
		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if(FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if(backBufferTexture)
			backBufferTexture->Release();

		if(FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}


		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		
		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);

		return true;			
		
	}

	void LiberaD3D(void)
	{
		if(depthTexture)
			depthTexture->Release();
		if(depthStencilView)
			depthStencilView->Release();
		if(backBufferTarget)
			backBufferTarget->Release();
		if(swapChain)
			swapChain->Release();
		if(d3dContext)
			d3dContext->Release();
		if(d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
	}
	
	void Render(void)
	{
		float sphere[3] = { 0,0,0 };
		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		static float angle = 0.0f;
		angle += 0.005;
		if (angle >= 360) angle = 0.0f;
		bool collide = false;
		if( d3dContext == 0 )
			return;

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView( backBufferTarget, clearColor );
		d3dContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 5 ;
		camara->UpdateCam(vel, arriba, izqder);
		skydome->Update(camara->vista, camara->proyeccion);

		float camPosXZ[2] = { camara->posCam.x, camara->posCam.z };

		TurnOffDepth();
		skydome->Render(camara->posCam);
		TurnOnDepth();
		terreno->Draw(camara->vista, camara->proyeccion);
		//TurnOnAlphaBlending();
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-11, -78, 4, 5, uv1, uv2, uv3, uv4, frameBillboard);

		//TurnOffAlphaBlending();
		model->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 1.0f, 0, 'A', 1);

		m01church->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-85.0f, 41.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m02bull->Draw(camara->vista, camara->proyeccion, terreno->Superficie(90.0f, -19.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m03houseEastern->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-10.0f, -129.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m04woodenCabinA->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-86.0f, -23.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m04woodenCabinB->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-86.0f, -23.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m05RuinedBuilding->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-50.0f, 44.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m06campfire->Draw(camara->vista, camara->proyeccion, terreno->Superficie(22.0f, 80.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m07tree->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-10.0f, -92.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m09molino->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-120.0f, 0.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m10flag->Draw(camara->vista, camara->proyeccion, terreno->Superficie(63.0f, -12.0f), camara->posCam, 10.0f, 0, 'A', 0.5f);
		m11barrel->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-23.5f, -6.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m12bride->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-153.0f, -100.0f), camara->posCam, 10.0f, 0, 'A', 0.1f);
		m13pozo->Draw(camara->vista, camara->proyeccion, terreno->Superficie(-20.0f, -36.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m14TreeMonster->Draw(camara->vista, camara->proyeccion, terreno->Superficie(10.0f, 10.0f), camara->posCam, 10.0f, 0, 'A', 0.5f);
		m15minibridge->Draw(camara->vista, camara->proyeccion, terreno->Superficie(37.0f, -105.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m16fountainA->Draw(camara->vista, camara->proyeccion, terreno->Superficie(10.0f, 10.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);
		m16fountainB->Draw(camara->vista, camara->proyeccion, terreno->Superficie(10.0f, 10.0f), camara->posCam, 10.0f, 0, 'A', 0.2f);

		m17castleA->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100.0f, -25.0f), camara->posCam, 10.0f, 0, 'A', 0.14f);
		m17castleB->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100.0f, -25.0f), camara->posCam, 10.0f, 0, 'A', 0.14f);
		m18MarketGray->Draw(camara->vista, camara->proyeccion, terreno->Superficie(9.0f, 45.0f), camara->posCam, 10.0f, 0, 'A', 0.6f);
		m19Rock->Draw(camara->vista, camara->proyeccion, terreno->Superficie(73.0f, 85.0f), camara->posCam, 10.0f, 0, 'A', 0.8f);



		swapChain->Present( 1, 0 );
	}

	bool isPointInsideSphere(float* point, float* sphere) {
		bool collition = false;

		float distance = sqrt((point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[1] - sphere[1]) * (point[1] - sphere[1]));

		if (distance < sphere[2])
			collition = true;
		return collition;
	}

	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}

	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable=true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);
		
		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable=true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}

	void billCargaFuego()
	{
		uv1[0].u = .125;
		uv2[0].u = .125;
		uv3[0].u = 0;
		uv4[0].u = 0;

		uv1[0].v = .25;
		uv2[0].v = 0;
		uv3[0].v = 0;
		uv4[0].v = .25;


		for (int j = 0; j < 8; j++) {
			uv1[j].u = uv1[0].u + (j * .125);
			uv2[j].u = uv2[0].u + (j * .125);
			uv3[j].u = uv3[0].u + (j * .125);
			uv4[j].u = uv4[0].u + (j * .125);

			uv1[j].v = .25;
			uv2[j].v = 0;
			uv3[j].v = 0;
			uv4[j].v = .25;
		}
		for (int j = 0; j < 8; j++) {
			uv1[j + 8].u = uv1[0].u + (j * .125);
			uv2[j + 8].u = uv2[0].u + (j * .125);
			uv3[j + 8].u = uv3[0].u + (j * .125);
			uv4[j + 8].u = uv4[0].u + (j * .125);

			uv1[j + 8].v = .5;
			uv2[j + 8].v = .25;
			uv3[j + 8].v = .25;
			uv4[j + 8].v = .5;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 16].u = uv1[0].u + (j * .125);
			uv2[j + 16].u = uv2[0].u + (j * .125);
			uv3[j + 16].u = uv3[0].u + (j * .125);
			uv4[j + 16].u = uv4[0].u + (j * .125);

			uv1[j + 16].v = .75;
			uv2[j + 16].v = .5;
			uv3[j + 16].v = .5;
			uv4[j + 16].v = .75;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 24].u = uv1[0].u + (j * .125);
			uv2[j + 24].u = uv2[0].u + (j * .125);
			uv3[j + 24].u = uv3[0].u + (j * .125);
			uv4[j + 24].u = uv4[0].u + (j * .125);

			uv1[j + 24].v = 1;
			uv2[j + 24].v = .75;
			uv3[j + 24].v = .75;
			uv4[j + 24].v = 1;
		}
	}

};
#endif