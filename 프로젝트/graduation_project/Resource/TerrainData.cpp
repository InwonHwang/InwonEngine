#include "TerrainData.h"

bool TerrainData::Create(void* pData, void* pResourceData, const string& name)
{
	_name = make_shared<string>(name);
	/*_vertices.reset(new HeightMapVec);
	_pRawData.reset(new vector<BYTE>);

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

	_pRawData->resize(verticeCount);
	stream.read((char*)&_pRawData->data()[0], verticeCount);

	stream.close();
	if (width % 2 == 0)
		createEven(pd3dDevice, width, *_pRawData);
	else
		createOdd(pd3dDevice, width, *_pRawData);*/

	return true;
}

void TerrainData::Destroy()
{
	_vertices.reset();
	_indices.reset();
	_vertices.reset();
	_pRawData.reset();
}

bool TerrainData::createEven(IDirect3DDevice9* pDevice, int width, const std::vector<BYTE>& height)
{
	//assert(width % 2 == 0);

	//_width = width + 1;
	//size_t verticeCount = _width * _width;

	//_vertices->resize(verticeCount);

	//int sizeVB = verticeCount * sizeof(STATICMESHVERTEX);
	//int sizeIB = (_width - 1) * (_width - 1) * 2 * sizeof(INDEX);
	//float coordIncrementSize = 1.0f / (float)_width;

	//VERTEXBUFFERINFO vi;
	//vi.stride = sizeof(STATICMESHVERTEX);
	//vi.verticeCount = verticeCount;
	//vi.fvf = STATICMESHVERTEX::FVF;
	//vi.usage = 0;
	//vi.pool = D3DPOOL_MANAGED;

	//_vertexBuffer = ResourceMgr->Create<VertexBuffer>(_name + "_vb", &vi);

	//INDEXBUFFERINFO ii;
	//ii.size = sizeIB;
	//ii.usage = 0;
	//ii.pool = D3DPOOL_DEFAULT;

	//_indexBuffer = ResourceMgr->Create<IndexBuffer>(_name + "_ib", &ii);

	//int indexVertex;
	//int indexHeightMap;

	//void* pData = nullptr;
	//if (_vertexBuffer->Lock(0, sizeVB, &pData, 0))
	//{
	//	STATICMESHVERTEX* pVertice = static_cast<STATICMESHVERTEX *>(pData);

	//	for (int z = 0; z < width; ++z)
	//	{
	//		for (int x = 0; x < width; ++x)
	//		{
	//			indexVertex = x + z * _width; //
	//			indexHeightMap = x + z * width;	 // HeightMap Index;

	//			STATICMESHVERTEX vertex;
	//			float posX = (float)(x - width / 2);
	//			float posZ = (float)(z - width / 2);
	//			BYTE posY = BYTE(height[indexHeightMap] * _height);
	//			vertex.position = Vector3(posX, posY, posZ);
	//			vertex.texCoord = Vector2(x * coordIncrementSize, z * coordIncrementSize);

	//			_maxHeight = fmaxf(_maxHeight, vertex.position.y);
	//			_minHeight = fminf(_minHeight, vertex.position.y);
	//			
	//			pVertice[indexVertex] = vertex;
	//			_vertices->data()[indexVertex] = vertex.position;
	//		}
	//	}

	//	for (int z = 0; z < _width; ++z)
	//	{
	//		indexVertex = width + z * _width;
	//		pVertice[width + z * _width].position = _vertices->data()[width + z * _width - 1] + Vector3(1, 0, 0);
	//		_vertices->data()[indexVertex] = pVertice[indexVertex].position;

	//		_maxHeight = fmaxf(_maxHeight, pVertice[indexVertex].position.y);
	//		_minHeight = fminf(_minHeight, pVertice[indexVertex].position.y);
	//	}

	//	for (int x = 0; x < _width; ++x)
	//	{
	//		indexVertex = x + width * _width;
	//		pVertice[indexVertex].position = _vertices->data()[x + (width - 1) * _width] + Vector3(0, 0, 1);
	//		_vertices->data()[indexVertex] = pVertice[indexVertex].position;

	//		_maxHeight = fmaxf(_maxHeight, pVertice[indexVertex].position.y);
	//		_minHeight = fminf(_minHeight, pVertice[indexVertex].position.y);
	//	}

	//	_vertexBuffer->Unlock();
	//}

	return true;
}

bool TerrainData::createOdd(IDirect3DDevice9* pDevice, int width, const std::vector<BYTE>& height)
{
	//assert(width % 2 != 0);

	//_width = width;
	//size_t verticeCount = _width * _width;

	//_vertices->resize(verticeCount);

	//int sizeVB = verticeCount * sizeof(STATICMESHVERTEX);
	//int sizeIB = (_width - 1) * (_width - 1) * 2 * sizeof(INDEX);
	//float coordIncrementSize = 1.0f / (float)_width;

	//VERTEXBUFFERINFO vi;
	//vi.stride = sizeof(STATICMESHVERTEX);
	//vi.verticeCount = verticeCount;
	//vi.fvf = STATICMESHVERTEX::FVF;
	//vi.usage = 0;
	//vi.pool = D3DPOOL_MANAGED;

	//_vertexBuffer = ResourceMgr->Create<VertexBuffer>(_name + "_vb", &vi);

	//INDEXBUFFERINFO ii;
	//ii.size = sizeIB;
	//ii.usage = 0;
	//ii.pool = D3DPOOL_DEFAULT;

	//_indexBuffer = ResourceMgr->Create<IndexBuffer>(_name + "_ib", &ii);

	//_vertices.reset(new vector<Vector3>());
	//_vertices->resize(verticeCount);

	//_indices.reset(new vector<INDEX>());

	//int indexVertex;
	//int indexHeightMap;

	//void* pData = nullptr;
	//if (_vertexBuffer->Lock(0, sizeVB, &pData, 0))
	//{
	//	STATICMESHVERTEX* pVertice = static_cast<STATICMESHVERTEX *>(pData);

	//	for (int z = 0; z < _width; ++z)
	//	{
	//		for (int x = 0; x < _width; ++x)
	//		{
	//			indexVertex = x + z * _width; //
	//			indexHeightMap = x + z * _width;	 // HeightMap Index;

	//			STATICMESHVERTEX vertex;
	//			float posX = (float)(x - width / 2);
	//			float posZ = (float)(z - width / 2);
	//			BYTE posY = BYTE(height[indexHeightMap] * _height);
	//			vertex.position = Vector3(posX, posY, posZ);
	//			vertex.normal = Vector3(0, 1, 0);
	//			vertex.texCoord = Vector2(x * coordIncrementSize, z * coordIncrementSize);

	//			_maxHeight = fmaxf(_maxHeight, vertex.position.y);
	//			_minHeight = fminf(_minHeight, vertex.position.y);

	//			pVertice[indexVertex] = vertex;
	//			_vertices->data()[indexVertex] = vertex.position;
	//		}
	//	}

	//	_vertexBuffer->Unlock();
	//}

	return true;
}