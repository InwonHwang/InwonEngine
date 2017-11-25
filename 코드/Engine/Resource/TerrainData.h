#pragma once
#include "GeometryResource.h"

class TerrainData : public GeometryResource
{
	typedef vector<Vector3> HeightMapVec;

private:
	shared_ptr<vector<BYTE>> _rawData;
	int			_width;
	float		_height;
	float		_minHeight;
	float		_maxHeight;

private:
	bool createEven(IDirect3DDevice9* pDevice, int width, const std::vector<BYTE>& height);
	bool createOdd(IDirect3DDevice9* pDevice, int width, const std::vector<BYTE>& height);

public:
	TerrainData();
	~TerrainData();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;
	virtual int GetStride() const override { return sizeof(STATICMESHVERTEX); }
	virtual unsigned long GetFVF() const override { return STATICMESHVERTEX::FVF; }

	int GetWidth() const { return _width; }
	float GetMinHeight() const { return _minHeight; }
	float GetMaxHeight() const { return _maxHeight; }
	shared_ptr<vector<BYTE>> GetRawData() const { return _rawData; }
};