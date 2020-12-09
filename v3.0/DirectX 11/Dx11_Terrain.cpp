#include "Dx11_Terrain.h"
#include <stdio.h>


bool Dx11_Terrain::LoadHeightMap(std::string _sHeightMapFile)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;


	// Open the height map file in binary.
	error = fopen_s(&filePtr, _sHeightMapFile.c_str(), "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	m_iWidth = bitmapInfoHeader.biWidth;
	m_iHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_iWidth * m_iHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Create the structure to hold the height map data.
	m_pHeightMap = new stHeightMap[m_iWidth * m_iHeight];
	if (!m_pHeightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	float fScalarValue = 10;
	// Read the image data into the height map.
	for (j = 0; j < m_iHeight; j++)
	{
		for (i = 0; i < m_iWidth; i++)
		{
			height = bitmapImage[k];

			index = (m_iHeight * j) + i;

			m_pHeightMap[index].x = (float)i;
			m_pHeightMap[index].y = ((float)height / 255.0) * fScalarValue;
			m_pHeightMap[index].z = (float)j;

			m_pHeightMap[index].tX = (float)i / (m_iWidth - 1);
			m_pHeightMap[index].tY = (float)j / (m_iHeight - 1);

			m_pHeightMap[index].nX = 1.0f;
			m_pHeightMap[index].nY = 1.0f;
			m_pHeightMap[index].nZ = 1.0f;

			k += 3;
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

Dx11_Terrain::Dx11_Terrain()
	:m_iWidth(15*10), 
	m_iHeight(10*10)
{

}


Dx11_Terrain::~Dx11_Terrain()
{
}

bool Dx11_Terrain::Init(ID3D11Device * _pDevice)
{	
	//
	this->LoadHeightMap("../../Data/heightmap01.bmp");

	m_iVertexCount = 6 * (m_iWidth -1)  * (m_iHeight-1)  ;	
	m_pVertexList = new stVertex[m_iVertexCount];

	unsigned int index = 0;
	for (int iCol = 0; iCol < m_iHeight-1; iCol++)
	{
		for (int iRow = 0; iRow < m_iWidth-1; iRow++)
		{
			unsigned int v1 = (m_iHeight * iCol) + iRow;
			unsigned int v2 = (m_iHeight * iCol) + (iRow + 1);
			unsigned int v3 = (m_iHeight * (iCol + 1)) + (iRow + 1);
			unsigned int v4 = (m_iHeight * (iCol + 1)) + (iRow);

			//First
			m_pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[v1].x, m_pHeightMap[v1].y, m_pHeightMap[v1].z);
			m_pVertexList[index].tex = D3DXVECTOR2(m_pHeightMap[v1].tX, m_pHeightMap[v1].tY);
			m_pVertexList[index].normal = D3DXVECTOR3(m_pHeightMap[v1].nX, m_pHeightMap[v1].nY, m_pHeightMap[v1].nZ);
			index++;

			m_pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[v2].x, m_pHeightMap[v2].y, m_pHeightMap[v2].z);
			m_pVertexList[index].tex = D3DXVECTOR2(m_pHeightMap[v2].tX, m_pHeightMap[v2].tY);
			m_pVertexList[index].normal = D3DXVECTOR3(m_pHeightMap[v2].nX, m_pHeightMap[v2].nY, m_pHeightMap[v2].nZ);
			index++;

			m_pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[v3].x, m_pHeightMap[v3].y, m_pHeightMap[v3].z);
			m_pVertexList[index].tex = D3DXVECTOR2(m_pHeightMap[v3].tX, m_pHeightMap[v3].tY);
			m_pVertexList[index].normal = D3DXVECTOR3(m_pHeightMap[v3].nX, m_pHeightMap[v3].nY, m_pHeightMap[v3].nZ);
			index++;

			//Second
			m_pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[v3].x, m_pHeightMap[v3].y, m_pHeightMap[v3].z);
			m_pVertexList[index].tex = D3DXVECTOR2(m_pHeightMap[v3].tX, m_pHeightMap[v3].tY);
			m_pVertexList[index].normal = D3DXVECTOR3(m_pHeightMap[v3].nX, m_pHeightMap[v3].nY, m_pHeightMap[v3].nZ);
			index++;

			m_pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[v4].x, m_pHeightMap[v4].y, m_pHeightMap[v4].z);
			m_pVertexList[index].tex = D3DXVECTOR2(m_pHeightMap[v4].tX, m_pHeightMap[v4].tY);
			m_pVertexList[index].normal = D3DXVECTOR3(m_pHeightMap[v4].nX, m_pHeightMap[v4].nY, m_pHeightMap[v4].nZ);
			index++;

			m_pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[v1].x, m_pHeightMap[v1].y, m_pHeightMap[v1].z);
			m_pVertexList[index].tex = D3DXVECTOR2(m_pHeightMap[v1].tX, m_pHeightMap[v1].tY);
			m_pVertexList[index].normal = D3DXVECTOR3(m_pHeightMap[v1].nX, m_pHeightMap[v1].nY, m_pHeightMap[v1].nZ);
			index++;
		}
	}
	
	/*
	if (index == m_iVertexCount)
	{
		float centerX = 0, centerZ = 0;
		for (int i = 0; i < m_iVertexCount; i++)
		{
			centerX += m_m_pVertexList[i].vPos.x;
			centerZ += m_m_pVertexList[i].vPos.z;
			char msgbuf[1000];
			sprintf_s(msgbuf, "\nVertex(%d) (%f, %f, %f)", i , m_m_pVertexList[i].vPos.x, m_m_pVertexList[i].vPos.y, m_m_pVertexList[i].vPos.z);
			OutputDebugStringA(msgbuf);

			if ((i + 1) % 6 == 0)
			{
				char Quad[10];
				sprintf_s(Quad, "\n");
				OutputDebugStringA(Quad);

			}
		}
		char msgbuf[1000];
		sprintf_s(msgbuf, "\Center (%f, %f, %f)", centerX/ m_iVertexCount, 0, centerZ / m_iVertexCount);
		OutputDebugStringA(msgbuf);
	}*/
	
 	return true;
}

void Dx11_Terrain::CopyVertices(void * _pVertexBuffer)
{	
	if(_pVertexBuffer != nullptr)
		memcpy(_pVertexBuffer, m_pVertexList, sizeof(stVertex)*m_iVertexCount);
}

void Dx11_Terrain::Release()
{
	delete m_pVertexList;
}
