//#pragma once
#ifndef _M16fountainB
#define _M16fountainB

//#define _XM_NO_INTRINSICS_

#include <d3d11.h>
#include <d3dx11.h>
#include <DxErr.h>
#include <D3Dcompiler.h>
#include <d3dx10math.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include "loadModel.h"
#include "Camara.h"
#include "M16fountainA.h"
#include "M14TreeMonster.h"
#include "waterShader/lightclass.h"
#include "waterShader/lightshaderclass.h"
#include "waterShader/refractionshaderclass.h"
#include "waterShader/rendertextureclass.h"
#include "waterShader/textureclass.h"
#include "waterShader/watershaderclass.h"

using namespace std;

class M16fountainB {
private:
	struct VertexComponent
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 UV;
		D3DXVECTOR3 normal;
	};

	struct VertexCollide
	{
		D3DXVECTOR3 pos;
	};

	struct vector3 {
		float x, y, z;
	};

	struct vector2 {
		float u, v;
	};

	ID3D11VertexShader* VertexShaderVS;
	ID3D11PixelShader* solidColorPS;

	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	
	ID3D11ShaderResourceView* colorMap;
	ID3D11ShaderResourceView* specMap;
	ID3D11SamplerState* colorMapSampler;

	ID3D11Buffer* viewCB;
	ID3D11Buffer* projCB;
	ID3D11Buffer* worldCB;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projMatrix;

	ID3D11Buffer* cameraPosCB;
	XMFLOAT3 camPos;
	ID3D11Buffer* specForceCB;
	float specForce;

	int ancho, alto;
	int anchoTexTerr, altoTexTerr;
	float anchof, altof;
	float deltax, deltay;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	HWND hwnd;

	
	CObjParser m_ObjParser;

	LightClass* m_Light;
	RenderTextureClass* m_RefractionTexture, * m_ReflectionTexture;
	LightShaderClass* m_LightShader;
	RefractionShaderClass* m_RefractionShader;
	WaterShaderClass* m_WaterShader;
	float m_waterHeight, m_waterTranslation;

	float posX;
	float posZ;

public:
	M16fountainB(ID3D11Device* D3DDevice, ID3D11DeviceContext* D3DContext,HWND hwnd, char* ModelPath, WCHAR* colorTexturePath, WCHAR* specularTexturePath, float _posX, float _posZ)
	{
		//copiamos el device y el device context a la clase terreno
		d3dContext = D3DContext;
		d3dDevice = D3DDevice;

		posX = _posX;
		posZ = _posZ;

		//aqui cargamos las texturas de alturas y el cesped
		CargaParametros(ModelPath,colorTexturePath, specularTexturePath);//L"Assets/Tent-Tower/tent_diffuse.jpg"
	}

	~M16fountainB()
	{
		//libera recursos

		UnloadContent();
	}

	float getPosX() {
		return this->posX;
	}

	float getPosZ() {
		return this->posZ;
	}

	bool CompileD3DShader(WCHAR* filePath, char* entry, char* shaderModel, ID3DBlob** buffer)
	{
		//forma de compilar el shader
		DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

		ID3DBlob* errorBuffer = 0;
		HRESULT d3dResult;

		d3dResult = D3DX11CompileFromFile(filePath, 0, 0, entry, shaderModel, shaderFlags,
			0, 0, buffer, &errorBuffer, 0);
		if (FAILED(d3dResult))
		{
			if (errorBuffer != 0)
			{
				OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
				errorBuffer->Release();
			}
			return false;
		}

		if (errorBuffer != 0)
			errorBuffer->Release();

		return true;
	}

	bool CargaParametros(char* ModelPath, WCHAR* diffuseTex, WCHAR* specularTex)
	{
		HRESULT d3dResult;

		ID3DBlob* vsBuffer = 0;

		//cargamos el shaders de vertices que esta contenido en el Shader.fx, note
		//que VS_Main es el nombre del vertex shader en el shader, vsBuffer contendra
		//al puntero del mismo
		bool compileResult = CompileD3DShader(L"ModeloTest3.fx", "VS_Main", "vs_4_0", &vsBuffer);
		//en caso de no poder cargarse ahi muere la cosa
		if (compileResult == false)
		{
			return false;
		}

		//aloja al shader en la memeoria del gpu o el cpu
		d3dResult = d3dDevice->CreateVertexShader(vsBuffer->GetBufferPointer(),
			vsBuffer->GetBufferSize(), 0, &VertexShaderVS);
		//en caso de falla sale
		if (FAILED(d3dResult))
		{

			if (vsBuffer)
				vsBuffer->Release();

			return false;
		}

		//lo nuevo, antes creabamos una estructura con la definicion del vertice, ahora creamos un mapa o layout
		//de como queremos construir al vertice, esto es la definicion
		D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

		d3dResult = d3dDevice->CreateInputLayout(solidColorLayout, totalLayoutElements,
			vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout);

		vsBuffer->Release();

		if (FAILED(d3dResult))
		{
			return false;
		}

		ID3DBlob* psBuffer = 0;
		// de los vertices pasamos al pixel shader, note que el nombre del shader es el mismo
		//ahora buscamos al pixel shader llamado PS_Main
		compileResult = CompileD3DShader(L"ModeloTest3.fx", "PS_Main", "ps_4_0", &psBuffer);

		if (compileResult == false)
		{
			return false;
		}
		//ya compilado y sin error lo ponemos en la memoria
		d3dResult = d3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(),
			psBuffer->GetBufferSize(), 0, &solidColorPS);

		psBuffer->Release();

		if (FAILED(d3dResult))
		{
			return false;
		}

		//aqui va la carga del modelo con el metodo creadoModelPath
		m_ObjParser.LoadFile(ModelPath); //"Assets/Tent-Tower/Tienda-Top.obj"
		//proceso de guardar el buffer de vertices para su uso en el render
		D3D11_BUFFER_DESC vertexDesc;
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(VertexObj) * m_ObjParser.m_nVertexCount;

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = m_ObjParser.m_pVertex;

		d3dResult = d3dDevice->CreateBuffer(&vertexDesc, &resourceData, &vertexBuffer);

		if (FAILED(d3dResult))
		{
			MessageBox(0, L"Error", L"Error al crear vertex buffer", MB_OK);
			return false;
		}


		//crea los accesos de las texturas para los shaders 
		d3dResult = D3DX11CreateShaderResourceViewFromFile(d3dDevice, diffuseTex, 0, 0, &colorMap, 0);
		d3dResult = D3DX11CreateShaderResourceViewFromFile(d3dDevice, specularTex, 0, 0, &specMap, 0);

		if (FAILED(d3dResult))
		{
			return false;
		}



		//aqui creamos el sampler
		D3D11_SAMPLER_DESC colorMapDesc;
		ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
		colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;
		//con la estructura de descripion creamos el sampler
		d3dResult = d3dDevice->CreateSamplerState(&colorMapDesc, &colorMapSampler);

		if (FAILED(d3dResult))
		{
			return false;
		}

		//creamos los buffers para el shader para poder pasarle las matrices
		D3D11_BUFFER_DESC constDesc;
		ZeroMemory(&constDesc, sizeof(constDesc));
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = sizeof(D3DXMATRIX);
		constDesc.Usage = D3D11_USAGE_DEFAULT;
		//de vista
		d3dResult = d3dDevice->CreateBuffer(&constDesc, 0, &viewCB);

		if (FAILED(d3dResult))
		{
			return false;
		}
		//de proyeccion
		d3dResult = d3dDevice->CreateBuffer(&constDesc, 0, &projCB);

		if (FAILED(d3dResult))
		{
			return false;
		}
		//de mundo
		d3dResult = d3dDevice->CreateBuffer(&constDesc, 0, &worldCB);

		if (FAILED(d3dResult))
		{
			return false;
		}

		constDesc.ByteWidth = sizeof(XMFLOAT4);
		d3dResult = d3dDevice->CreateBuffer(&constDesc, 0, &cameraPosCB);

		d3dResult = d3dDevice->CreateBuffer(&constDesc, 0, &specForceCB);

		if (FAILED(d3dResult))
		{
			return false;
		}

		//posicion de la camara
		D3DXVECTOR3 eye = D3DXVECTOR3(0.0f, 100.0f, 200.0f);
		//a donde ve
		D3DXVECTOR3 target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//crea la matriz de vista
		D3DXMatrixLookAtLH(&viewMatrix, &eye, &target, &up);
		//la de proyeccion
		D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI / 4.0, ancho / alto, 0.01f, 1000.0f);
		//las transpone para acelerar la multiplicacion
		D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
		D3DXMatrixTranspose(&projMatrix, &projMatrix);

		// Create the light object.
		m_Light = new LightClass;
		if (!m_Light)
		{
			return false;
		}

		// Initialize the light object.
		m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
		m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetDirection(0.0f, -1.0f, 0.5f);

		m_RefractionTexture = new RenderTextureClass;
		if (!m_RefractionTexture)
		{
			return false;
		}

		// Initialize the refraction render to texture object.
		d3dResult = m_RefractionTexture->Initialize(d3dDevice, 100, 100);
		if (!d3dResult)
		{
			MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
			return false;
		}

		// Create the reflection render to texture object.
		m_ReflectionTexture = new RenderTextureClass;
		if (!m_ReflectionTexture)
		{
			return false;
		}

		// Initialize the reflection render to texture object.
		d3dResult = m_ReflectionTexture->Initialize(d3dDevice, 100, 100);
		if (!d3dResult)
		{
			MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
			return false;
		}

		// Create the light shader object.
		m_LightShader = new LightShaderClass;
		if (!m_LightShader)
		{
			return false;
		}

		// Initialize the light shader object.
		d3dResult = m_LightShader->Initialize(d3dDevice, hwnd);
		if (!d3dResult)
		{
			MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
			return false;
		}

		// Create the refraction shader object.
		m_RefractionShader = new RefractionShaderClass;
		if (!m_RefractionShader)
		{
			return false;
		}

		// Initialize the refraction shader object.
		d3dResult = m_RefractionShader->Initialize(d3dDevice, hwnd);
		if (!d3dResult)
		{
			MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
			return false;
		}

		// Create the water shader object.
		m_WaterShader = new WaterShaderClass;
		if (!m_WaterShader)
		{
			return false;
		}

		// Initialize the water shader object.
		d3dResult = m_WaterShader->Initialize(d3dDevice, hwnd);
		if (!d3dResult)
		{
			MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
			return false;
		}

		// Set the height of the water.
		m_waterHeight = 2.75f;

		// Initialize the position of the water.
		m_waterTranslation = 0.0f;

		return true;
	}

	bool UnloadContent()
	{
		if (colorMapSampler)
			colorMapSampler->Release();
		if (colorMap)
			colorMap->Release();
		if (specMap)
			specMap->Release();
		if (VertexShaderVS)
			VertexShaderVS->Release();
		if (solidColorPS)
			solidColorPS->Release();
		if (inputLayout)
			inputLayout->Release();
		if (vertexBuffer)
			vertexBuffer->Release();
		if (viewCB)
			viewCB->Release();
		if (projCB)
			projCB->Release();
		if (worldCB)
			worldCB->Release();

		if (cameraPosCB)
			cameraPosCB->Release();
		if (specForceCB)
			specForceCB->Release();

		if (m_WaterShader)
		{
			m_WaterShader->Shutdown();
			delete m_WaterShader;
			m_WaterShader = 0;
		}

		// Release the refraction shader object.
		if (m_RefractionShader)
		{
			m_RefractionShader->Shutdown();
			delete m_RefractionShader;
			m_RefractionShader = 0;
		}

		// Release the light shader object.
		if (m_LightShader)
		{
			m_LightShader->Shutdown();
			delete m_LightShader;
			m_LightShader = 0;
		}

		// Release the reflection render to texture object.
		if (m_ReflectionTexture)
		{
			m_ReflectionTexture->Shutdown();
			delete m_ReflectionTexture;
			m_ReflectionTexture = 0;
		}

		// Release the refraction render to texture object.
		if (m_RefractionTexture)
		{
			m_RefractionTexture->Shutdown();
			delete m_RefractionTexture;
			m_RefractionTexture = 0;
		}

		// Release the light object.
		if (m_Light)
		{
			delete m_Light;
			m_Light = 0;
		}


		colorMapSampler = 0;
		colorMap = 0;
		specMap = 0;
		VertexShaderVS = 0;
		solidColorPS = 0;
		inputLayout = 0;
		vertexBuffer = 0;
		indexBuffer = 0;
		viewCB = 0;
		projCB = 0;
		worldCB = 0;
		cameraPosCB = 0;
		specForceCB = 0;
	}

	void Update()
	{
		
	}

	void Draw(D3DXMATRIX vista, D3DXMATRIX proyeccion, Camara* m_Camera, float ypos, D3DXVECTOR3 posCam, float specForce, float rot, char angle, float scale)
	{
		D3DMATRIX reflectionMatrix;
		static float rotation = 0.0f;
		rotation += 0.01;

		//paso de datos, es decir cuanto es el ancho de la estructura
		unsigned int stride = sizeof(VertexObj);
		unsigned int offset = 0;

		camPos.x = posCam.x;
		camPos.y = posCam.y;
		camPos.z = posCam.z;

		//define la estructura del vertice a traves de layout
		d3dContext->IASetInputLayout(inputLayout);

		//define con que buffer trabajara
		d3dContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		//define la forma de conexion de los vertices
		d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//Establece el vertex y pixel shader que utilizara
		d3dContext->VSSetShader(VertexShaderVS, 0, 0);
		d3dContext->PSSetShader(solidColorPS, 0, 0);
		//pasa lo sbuffers al shader
		d3dContext->PSSetShaderResources(0, 1, &colorMap);
		d3dContext->PSSetShaderResources(1, 1, &specMap);

		d3dContext->PSSetSamplers(0, 1, &colorMapSampler);

		//mueve la camara
		D3DXMATRIX rotationMat;
		D3DXMatrixRotationYawPitchRoll(&rotationMat, 0.0f, 0.0f, 0.0f);
		D3DXMATRIX translationMat;
		D3DXMatrixTranslation(&translationMat, posX, ypos, posZ);
		if (angle == 'X')
			D3DXMatrixRotationX(&rotationMat, rot);
		else if (angle == 'Y')
			D3DXMatrixRotationY(&rotationMat, rot);
		else if (angle == 'Z')
			D3DXMatrixRotationZ(&rotationMat, rot);
		viewMatrix *= rotationMat;

		D3DXMATRIX scaleMat;
		D3DXMatrixScaling(&scaleMat, scale, scale, scale);

		D3DXMATRIX worldMat = rotationMat * scaleMat * translationMat;
		D3DXMatrixTranspose(&worldMat, &worldMat);
		//actualiza los buffers del shader
		d3dContext->UpdateSubresource(worldCB, 0, 0, &worldMat, 0, 0);
		d3dContext->UpdateSubresource(viewCB, 0, 0, &vista, 0, 0);
		d3dContext->UpdateSubresource(projCB, 0, 0, &proyeccion, 0, 0);
		d3dContext->UpdateSubresource(cameraPosCB, 0, 0, &camPos, 0, 0);
		d3dContext->UpdateSubresource(specForceCB, 0, 0, &specForce, 0, 0);
		//le pasa al shader los buffers
		d3dContext->VSSetConstantBuffers(0, 1, &worldCB);
		d3dContext->VSSetConstantBuffers(1, 1, &viewCB);
		d3dContext->VSSetConstantBuffers(2, 1, &projCB);
		d3dContext->VSSetConstantBuffers(3, 1, &cameraPosCB);
		d3dContext->VSSetConstantBuffers(4, 1, &specForceCB);
		//cantidad de trabajos

		d3dContext->Draw(m_ObjParser.m_nVertexCount, 0);

		reflectionMatrix = m_Camera->GetReflectionViewMatrix();


		m_WaterShader->Render(d3dContext, m_ObjParser.m_nVertexCount, worldMat, viewMatrix,
			proyeccion, reflectionMatrix, m_ReflectionTexture->GetShaderResourceView(),
			m_RefractionTexture->GetShaderResourceView(), specMap,
			m_waterTranslation, 0.01f);
		

	


	}

	//bool RenderRefractionToTexture(Camara *m_Camera, M16fountainA* Fuente, 
	//		ID3D11RenderTargetView* backBufferTarget, ID3D11DepthStencilView* DepthStencil, 
	//		D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj)
	//{
	//	D3DXVECTOR4 clipPlane;
	//	D3DXMATRIX worldMatrix = world, viewMatrix = view, projectionMatrix = proj;
	//	bool result;


	//	// Setup a clipping plane based on the height of the water to clip everything above it.
	//	clipPlane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	//	// Set the render target to be the refraction render to texture.
	//	m_RefractionTexture->SetRenderTarget(d3dContext, DepthStencil);

	//	// Clear the refraction render to texture.
	//	m_RefractionTexture->ClearRenderTarget(d3dContext, DepthStencil, 0.0f, 0.0f, 0.0f, 1.0f);

	//	// Get the world, view, and projection matrices from the camera and d3d objects.
	//

	//	// Translate to where the bath model will be rendered.
	//	D3DXMatrixTranslation(&worldMatrix, -10.0f, 2.0f, -50.0f);


	//	// Render the bath model using the light shader.
	//	result = m_RefractionShader->Render(d3dContext, Fuente->GetIndexCount(), worldMatrix, viewMatrix,
	//		projectionMatrix, Fuente->GetTexture(), m_Light->GetDirection(),
	//		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
	//	if (!result)
	//	{
	//		return false;
	//	}

	//	// Reset the render target back to the original back buffer and not the render to texture anymore.
	//	d3dContext->OMSetRenderTargets(1, &backBufferTarget, DepthStencil);

	//	return true;
	//}


	//bool RenderReflectionToTexture(Camara *m_Camera, M14TreeMonster* Arbol, ID3D11RenderTargetView* backBufferTarget, 
	//			ID3D11DepthStencilView* DepthStencil, D3DXMATRIX world, D3DXMATRIX proj)
	//{
	//	D3DXMATRIX reflectionViewMatrix, worldMatrix = world, projectionMatrix = proj;
	//	bool result;


	//	// Set the render target to be the reflection render to texture.
	//	m_ReflectionTexture->SetRenderTarget(d3dContext, DepthStencil);

	//	// Clear the reflection render to texture.
	//	m_ReflectionTexture->ClearRenderTarget(d3dContext, DepthStencil, 0.0f, 0.0f, 0.0f, 1.0f);

	//	// Use the camera to render the reflection and create a reflection view matrix.
	//	m_Camera->RenderReflection(m_waterHeight);

	//	// Get the camera reflection view matrix instead of the normal view matrix.
	//	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();


	//	// Translate to where the wall model will be rendered.
	//	D3DXMatrixTranslation(&worldMatrix, 0.0f, 6.0f, -50.0f);


	//	// Render the wall model using the light shader and the reflection view matrix.
	//	result = m_LightShader->Render(d3dContext, Arbol->GetIndexCount(), worldMatrix, reflectionViewMatrix,
	//		projectionMatrix, Arbol->GetTexture(), m_Light->GetDirection(),
	//		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	//	if (!result)
	//	{
	//		return false;
	//	}

	//	// Reset the render target back to the original back buffer and not the render to texture anymore.
	//	d3dContext->OMSetRenderTargets(1, &backBufferTarget, DepthStencil);

	//	return true;
	//}

};
#endif

