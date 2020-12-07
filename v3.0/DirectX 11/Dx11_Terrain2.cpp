#include "Dx11_Terrain2.h"

bool Dx11_Terrain2::InitShader(ID3D11Device* _pDevice)
{
	ID3D10Blob* pError, * pVSBuffer, * pPSBuffer;

	HRESULT hr = D3DX11CompileFromFile(L"../../Data/TerrainShader.hlsl", NULL, NULL, "MyVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &pVSBuffer, &pError, NULL);
	if (hr != S_OK)
	{
		FILE* fp = NULL;
		char* pCompileError = (char*)(pError->GetBufferPointer());
		unsigned long bufferSize = pError->GetBufferSize();

		fopen_s(&fp, "../../Data/ShaderCompileError.txt", "w");
		if (fp)
		{
			for (int i = 0; i < bufferSize; i++)
			{
				fwrite(&pCompileError[i], sizeof(pCompileError[i]), 1, fp);
			}
			fclose(fp);
		}
		return false;
	}


	hr = D3DX11CompileFromFile(L"../../Data/TerrainShader.hlsl", NULL, NULL, "MyPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &pPSBuffer, &pError, NULL);
	if (hr != S_OK)
		return false;


	hr = _pDevice->CreateVertexShader(pVSBuffer->GetBufferPointer(), pVSBuffer->GetBufferSize(), NULL, &m_pVS);
	if (hr != S_OK)
		return false;

	hr = _pDevice->CreatePixelShader(pPSBuffer->GetBufferPointer(), pPSBuffer->GetBufferSize(), NULL, &m_pPS);
	if (hr != S_OK)
		return false;


	D3D11_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	UINT numElements = ARRAYSIZE(inputLayout);
	hr = _pDevice->CreateInputLayout(inputLayout, numElements, pVSBuffer->GetBufferPointer(), pVSBuffer->GetBufferSize(), &m_pInputLayout);
	if (hr != S_OK)
		return false;

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(stWVPBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	hr = _pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pWVPBuffer);
	if (hr != S_OK)
		return false;

	

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = _pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	if (hr != S_OK)
		return false;

	return true;
}

bool Dx11_Terrain2::InitBuffer(ID3D11Device* _pDevice)
{		
	//m_uiWidth = 3;
	//m_uiHeight = 2;
	m_nVertexCount = 12 * (m_uiWidth - 1) * (m_uiHeight - 1);
	stVertex* pVertexList = new stVertex[m_nVertexCount];	
	
	m_nIndexCount = m_nVertexCount;
	unsigned int* pIndexList = new unsigned int[m_nIndexCount];

	unsigned int index = 0;
	for (unsigned int iCol = 0; iCol < m_uiHeight - 1; iCol++)
	{
		for (unsigned int iRow = 0; iRow < m_uiWidth - 1; iRow++)
		{
			unsigned int index1 = (m_uiHeight * iCol) + iRow;          // Bottom left.
			unsigned int index2 = (m_uiHeight * iCol) + (iRow + 1);      // Bottom right.
			unsigned int index3 = (m_uiHeight * (iCol + 1)) + iRow;      // Upper left.
			unsigned int index4 = (m_uiHeight * (iCol + 1)) + (iRow + 1);  // Upper right.
						
			//1 Upper left.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index3].x, m_pHeightMap[index3].y, m_pHeightMap[index3].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			
			pIndexList[index] = index;
			index++;

			//2 Upper right.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index4].x, m_pHeightMap[index4].y, m_pHeightMap[index4].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;

			//3 Upper right.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index4].x, m_pHeightMap[index4].y, m_pHeightMap[index4].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;

			//4 Bottom left.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index1].x, m_pHeightMap[index1].y, m_pHeightMap[index1].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;

			//5 Bottom left.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index1].x, m_pHeightMap[index1].y, m_pHeightMap[index1].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;

			//6 Upper left.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index3].x, m_pHeightMap[index3].y, m_pHeightMap[index3].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;

			//7 Bottom left.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index1].x, m_pHeightMap[index1].y, m_pHeightMap[index1].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;

			//8 Upper right.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index4].x, m_pHeightMap[index4].y, m_pHeightMap[index4].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;

			//9 Upper right.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index4].x, m_pHeightMap[index4].y, m_pHeightMap[index4].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;

			//10 Bottom right.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index2].x, m_pHeightMap[index2].y, m_pHeightMap[index2].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;

			//11 Bottom right.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index2].x, m_pHeightMap[index2].y, m_pHeightMap[index2].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;

			//12 Bottom left.
			pVertexList[index].pos = D3DXVECTOR3(m_pHeightMap[index1].x, m_pHeightMap[index1].y, m_pHeightMap[index1].z);
			pVertexList[index].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertexList[index].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			pIndexList[index] = index;
			index++;
		}
	}

	D3D11_BUFFER_DESC vertDesc, indxDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	ZeroMemory(&vertDesc, sizeof(vertDesc));
	vertDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertDesc.ByteWidth = sizeof(stVertex) * m_nVertexCount;
	vertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertDesc.MiscFlags = 0;
	vertDesc.StructureByteStride = 0;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = pVertexList;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	HRESULT hr = _pDevice->CreateBuffer(&vertDesc, &vertexData, &m_pVertexBuffer);
	if (hr != S_OK)
		return false;

	ZeroMemory(&indxDesc, sizeof(indxDesc));
	indxDesc.Usage = D3D11_USAGE_DYNAMIC;
	indxDesc.ByteWidth = sizeof(unsigned int) * m_nIndexCount;
	indxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indxDesc.MiscFlags = 0;
	indxDesc.StructureByteStride = 0;

	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = pIndexList;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	hr = _pDevice->CreateBuffer(&indxDesc, &indexData, &m_pIndexBuffer);
	if (hr != S_OK)
		return false;


	delete[] pVertexList;
	delete[] pIndexList;
	pVertexList = 0;
	pIndexList = 0;

	return true;
}

bool Dx11_Terrain2::LoadHeightMap(std::string _sHeightMapFile)
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
	m_uiWidth = bitmapInfoHeader.biWidth;
	m_uiHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_uiWidth * m_uiHeight * 3;

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
	m_pHeightMap = new stHeightMap[m_uiWidth * m_uiHeight];
	if (!m_pHeightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j < m_uiHeight; j++)
	{
		for (i = 0; i < m_uiWidth; i++)
		{
			height = bitmapImage[k];

			index = (m_uiHeight * j) + i;

			m_pHeightMap[index].x = (float)i;
			m_pHeightMap[index].y = (float)height / 15.0;
			m_pHeightMap[index].z = (float)j;

			k += 3;
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

Dx11_Terrain2::Dx11_Terrain2()
	:m_uiWidth(0),
	m_uiHeight(0),
	m_nVertexCount(0),
	m_nIndexCount(0),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pWVPBuffer(nullptr),
	m_pVS(nullptr),
	m_pPS(nullptr),
	m_pInputLayout(nullptr),
	m_pSamplerState(nullptr),
	m_pHeightMap(nullptr)
{
}

Dx11_Terrain2::~Dx11_Terrain2()
{
}

bool Dx11_Terrain2::Init(ID3D11Device* _pDevice)
{
	if (this->InitShader(_pDevice))
	{
		//First need to be initialize height map before init buffers
		if(this->LoadHeightMap("../../Data/heightmap01.bmp"))
			return this->InitBuffer(_pDevice);
	}
    return false;
}

void Dx11_Terrain2::Render(ID3D11DeviceContext* _pDeviceContext, D3DXMATRIX _worldMat, D3DXMATRIX _viewMat, D3DXMATRIX _projMat, D3DXVECTOR3 _vCamPos)
{
	_pDeviceContext->IASetInputLayout(m_pInputLayout);

	//SetVertex Buffer
	UINT stride = sizeof(stVertex);
	UINT offset = 0;
	_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);// Rendering as point.
	//_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	
	//Set Buffer To Vertex Shader
	D3DXMatrixTranspose(&_worldMat, &_worldMat);
	D3DXMatrixTranspose(&_viewMat, &_viewMat);
	D3DXMatrixTranspose(&_projMat, &_projMat);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = _pDeviceContext->Map(m_pWVPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (hr != S_OK)
		return;
	stWVPBuffer* pData = (stWVPBuffer*)mappedResource.pData;
	pData->worldMat = _worldMat;
	pData->viewMat = _viewMat;
	pData->projMat = _projMat;
	_pDeviceContext->Unmap(m_pWVPBuffer, 0);
	_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pWVPBuffer);
	
	_pDeviceContext->VSSetShader(m_pVS, nullptr, 0);
	_pDeviceContext->PSSetShader(m_pPS, nullptr, 0);
	_pDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
}
