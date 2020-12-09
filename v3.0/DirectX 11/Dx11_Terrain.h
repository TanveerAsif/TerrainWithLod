#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <string>

class Dx11_Terrain
{
public:
	struct  stVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
		D3DXVECTOR3 normal;
	};
	unsigned int m_iVertexCount;


private:
	unsigned int m_iWidth, m_iHeight;
	stVertex	*m_pVertexList;
	bool					LoadHeightMap(std::string _sHeightMapFile);

	struct stHeightMap
	{
		float x, y, z;
		float tX, tY;
		float nX;
		float nY;
		float nZ;
	};
	stHeightMap* m_pHeightMap;

public:
	Dx11_Terrain();
	~Dx11_Terrain();

	bool			Init(ID3D11Device *_pDevice);
	unsigned int	GetVertexCount() { return m_iVertexCount; }
	void			CopyVertices(void *pVertexBuffer);
	void			Release();
};

