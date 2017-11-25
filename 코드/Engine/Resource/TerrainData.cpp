#include "TerrainData.h"

TerrainData::TerrainData()
	: GeometryResource(),
	_width(0),
	_height(0.3f),
	_minHeight(Mathf::Infinity()),
	_maxHeight(-Mathf::Infinity())
{
	_rawData = make_shared<vector<BYTE>>();
}

TerrainData::~TerrainData()
{
}

bool TerrainData::Create(void* pData, void* pResourceData, const string& name)
{
	*_name = name;

	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
	string* path = static_cast<string*>(pResourceData);

	std::ifstream stream;
	stream.open(path->c_str(), std::ios::binary);	

	if (stream.fail())
		return false;

	stream.seekg(0, std::ios::end);
	int verticeCount = (int)stream.tellg();
	stream.seekg(0, std::ios::beg);


	int width = (int)Mathf::Sqrt((float)verticeCount);

	_rawData->resize(verticeCount);
	stream.read((char*)&_rawData->data()[0], verticeCount);

	stream.close();
	if (width % 2 == 0)
		createEven(pd3dDevice, width, *_rawData);
	else
		createOdd(pd3dDevice, width, *_rawData);

	return true;
}

void TerrainData::Destroy()
{
	SafeRelease<IDirect3DVertexBuffer9>(_pVertexBuffer);
	SafeRelease<IDirect3DIndexBuffer9>(_pIndexBuffer);
}

bool TerrainData::createEven(IDirect3DDevice9* pDevice, int width, const std::vector<BYTE>& height)
{
	_width = width + 1;
	size_t verticeCount = _width * _width;

	_vertices->resize(verticeCount);

	int sizeVB = verticeCount * sizeof(STATICMESHVERTEX);
	int sizeIB = (_width - 1) * (_width - 1) * 2 * sizeof(INDEX);
	float coordIncrementSize = 1.0f / (float)_width;

	if (FAILED(pDevice->CreateVertexBuffer(sizeVB,
		0, STATICMESHVERTEX::FVF, D3DPOOL_MANAGED, &_pVertexBuffer, NULL))) {
		return false;
	}

	if (FAILED(pDevice->CreateIndexBuffer(sizeIB,
		0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &_pIndexBuffer, 0)))
	{
		SafeRelease<IDirect3DVertexBuffer9>(_pVertexBuffer);
		return false;
	}

	void* pData = nullptr;
	if (_pVertexBuffer->Lock(0, sizeVB, &pData, 0))
	{
		STATICMESHVERTEX* pVertice = static_cast<STATICMESHVERTEX *>(pData);

		for (int z = 0; z < width; ++z)
		{
			for (int x = 0; x < width; ++x)
			{
				int idx = x + z * _width; //
				int heightMapIdx = x + z * width;	 // HeightMap Index;

				STATICMESHVERTEX vertex;
				float posX = (float)(x - width / 2);
				float posZ = (float)(z - width / 2);
				BYTE posY = BYTE(height[heightMapIdx] * _height);
				vertex.position = Vector3(posX, posY, posZ);
				vertex.texCoord = Vector2(x * coordIncrementSize, z * coordIncrementSize);

				_maxHeight = fmaxf(_maxHeight, vertex.position.y);
				_minHeight = fminf(_minHeight, vertex.position.y);
				
				pVertice[idx] = vertex;
				_vertices->data()[idx] = vertex.position;
			}
		}

		for (int z = 0; z < _width; ++z)
		{
			int idx = width + z * _width;
			pVertice[width + z * _width].position = _vertices->data()[width + z * _width - 1] + Vector3(1, 0, 0);
			_vertices->data()[idx] = pVertice[idx].position;

			_maxHeight = fmaxf(_maxHeight, pVertice[idx].position.y);
			_minHeight = fminf(_minHeight, pVertice[idx].position.y);
		}

		for (int x = 0; x < _width; ++x)
		{
			int idx = x + width * _width;
			pVertice[idx].position = _vertices->data()[x + (width - 1) * _width] + Vector3(0, 0, 1);
			_vertices->data()[idx] = pVertice[idx].position;

			_maxHeight = fmaxf(_maxHeight, pVertice[idx].position.y);
			_minHeight = fminf(_minHeight, pVertice[idx].position.y);
		}

		_pVertexBuffer->Unlock();
	}

	return true;
}

bool TerrainData::createOdd(IDirect3DDevice9* pDevice, int width, const std::vector<BYTE>& height)
{
	_width = width;
	size_t verticeCount = _width * _width;

	_vertices->resize(verticeCount);

	int sizeVB = verticeCount * sizeof(STATICMESHVERTEX);
	int sizeIB = (_width - 1) * (_width - 1) * 2 * sizeof(INDEX);
	float coordIncrementSize = 1.0f / (float)_width;

	if (FAILED(pDevice->CreateVertexBuffer(sizeVB,
		0, STATICMESHVERTEX::FVF, D3DPOOL_MANAGED, &_pVertexBuffer, NULL))) {
		return false;
	}

	if (FAILED(pDevice->CreateIndexBuffer(sizeIB,
		0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &_pIndexBuffer, 0)))
	{
		SafeRelease<IDirect3DVertexBuffer9>(_pVertexBuffer);
		return false;
	}

	int idx;
	int heightMapIdx;

	void* pData = nullptr;
	if (_pVertexBuffer->Lock(0, sizeVB, &pData, 0))
	{
		STATICMESHVERTEX* pVertice = static_cast<STATICMESHVERTEX *>(pData);

		for (int z = 0; z < _width; ++z)
		{
			for (int x = 0; x < _width; ++x)
			{
				idx = x + z * _width; //
				heightMapIdx = x + z * _width;	 // HeightMap Index;

				STATICMESHVERTEX vertex;
				float posX = (float)(x - width / 2);
				float posZ = (float)(z - width / 2);
				BYTE posY = BYTE(height[heightMapIdx] * _height);
				vertex.position = Vector3(posX, posY, posZ);
				vertex.normal = Vector3(0, 1, 0);
				vertex.texCoord = Vector2(x * coordIncrementSize, z * coordIncrementSize);

				_maxHeight = fmaxf(_maxHeight, vertex.position.y);
				_minHeight = fminf(_minHeight, vertex.position.y);

				pVertice[idx] = vertex;
				_vertices->data()[idx] = vertex.position;
			}
		}

		_pVertexBuffer->Unlock();
	}

	return true;
}