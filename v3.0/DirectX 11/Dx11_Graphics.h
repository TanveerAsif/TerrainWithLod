#pragma once
#include "Dx11_Direct3D.h"
#include "Dx11_Camera.h"
#include "Dx11_Model.h"
#include "Dx11_Shaders.h"
#include "Dx11_Font.h"
#include "Dx11_Assimp.h"
#include "Dx11_VolumeRendering.h"
#include "Dx11_Render2Texture.h"
#include "Dx11_Tessellation.h"

#include "Dx11_Terrain.h"
#include "Dx11_Terrain2.h"
#include "Dx11_QuadTree.h"


#ifdef VISUALIZATION_EXPORTS
#define VISUALIZATION_API __declspec(dllexport)
#else
#define VISUALIZATION_API __declspec(dllimport)
#endif

//#define MAX_NO_VOL 1


class /*VISUALIZATION_API*/ Dx11_Graphics
{
	
private:
	Dx11_Direct3D			*m_pDirect3D = NULL;
	Dx11_Camera				*m_pCamera = NULL;
	Dx11_Text				*m_pText = NULL;
	Dx11_Assimp				*m_pAssimp = NULL;	
	Dx11_Tessellation		*m_pTessellation = nullptr;
	Dx11_Terrain			*m_pTerrain = nullptr;
	Dx11_Terrain2			*m_pTerrain2 = nullptr;
	Dx11_QuadTree			*m_pQuadTree = nullptr;

	UINT					m_nScreenWidth, m_nScreenHeight;

	float					m_fTickCount;
	bool					m_bRenderState;
	bool					m_bIsWireFrameEnable;
	bool					m_bSunLightEnable;
	bool					m_bLightShaftEnable;
	bool					m_bShowMapEnable;

	float					m_fHeading = 0.0f;

	unsigned	int			m_uScaleWidth = 0, m_uScaleHeight = 0, m_uScaleDepth = 0;


	void					RenderScene(float _fTick);
public:
	Dx11_Graphics();
	~Dx11_Graphics();
	bool					Initialize(HWND hWnd, UINT _width, UINT _height);
	void					Process(float _fTick);
	void					Render(float _fTick);	
	void					Shutdown();
	void					UpdateCamera(char action);	
	void					ShowWireFrame();	
	void					SetTransferValue(float _fValue);
	void					CameraTransform(D3DXVECTOR2 _vScreenPoint);
	void					SetCameraLastDragPoint(D3DXVECTOR2 _vPoint);
	void					EnableShadowMap();
	void					SetTessellationFactor(int _iFactor);
	
};

