#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <string>

//Render the Terrain : instead of Dx11_Terrain
class Dx11_Texture;
class Dx11_Terrain2
{
	
private:
	struct  stVertex
	{
		D3DXVECTOR3 pos;		
		D3DXVECTOR2 tex;
		D3DXVECTOR3 normal;
	};

	struct stWVPBuffer
	{
		D3DXMATRIX worldMat;
		D3DXMATRIX viewMat;
		D3DXMATRIX projMat;
	};

	struct stHeightMap
	{
		float x, y, z;
		float tX, tY;
		float nX;
		float nY;
		float nZ;
	};
	stHeightMap*			m_pHeightMap;
	


	unsigned int			m_uiWidth, m_uiHeight;
	ID3D11Buffer			*m_pVertexBuffer, *m_pIndexBuffer, *m_pWVPBuffer;
	unsigned int            m_nVertexCount, m_nIndexCount;
	ID3D11VertexShader		*m_pVS;
	ID3D11PixelShader		*m_pPS;
	ID3D11InputLayout		*m_pInputLayout;
	ID3D11SamplerState		*m_pSamplerState;

	bool					InitShader(ID3D11Device* _pDevice);
	bool					InitBuffer(ID3D11Device* _pDevice);
	bool					LoadHeightMap(std::string _sHeightMapFile);
	
	Dx11_Texture			*m_pTexture;

public:
	Dx11_Terrain2();
	~Dx11_Terrain2();
	

	bool Init(ID3D11Device* _pDevice);
	void Render(ID3D11DeviceContext* _pDeviceContext, D3DXMATRIX _worldMat, D3DXMATRIX _viewMat, D3DXMATRIX _projMat, D3DXVECTOR3 _vCamPos);
};

