#pragma once

#include "GeometryResource.h"

class TerrainData : public GeometryResource
{
	typedef vector<Vector3> HeightMapVec;
public:
	TerrainData() : GeometryResource(),
		_width(0),
		_height(0.3f),
		_minHeight(Mathf::Infinity()),
		_maxHeight(-Mathf::Infinity())
	{
	}

	~TerrainData() {}

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;
	virtual int GetStride() const override { return sizeof(STATICMESHVERTEX); }
	virtual unsigned long GetFVF() const override { return STATICMESHVERTEX::FVF; }

	int GetWidth() const;
	float GetMinHeight() const;
	float GetMaxHeight() const;
	shared_ptr<vector<BYTE>> GetRawData() const;

private:
	bool createEven(IDirect3DDevice9* pDevice, int width, const std::vector<BYTE>& height);
	bool createOdd(IDirect3DDevice9* pDevice, int width, const std::vector<BYTE>& height);

private:
	shared_ptr<vector<BYTE>>	_pRawData;
	int					_width;
	float				_height;
	float				_minHeight;
	float				_maxHeight;
};

inline int TerrainData::GetWidth() const
{
	return _width;
}

inline float TerrainData::GetMinHeight() const
{
	return _minHeight;
}

inline float TerrainData::GetMaxHeight() const
{
	return _maxHeight;
}

inline shared_ptr<vector<BYTE>> TerrainData::GetRawData() const
{
	return _pRawData;
}