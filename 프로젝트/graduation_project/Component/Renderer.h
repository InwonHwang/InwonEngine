#pragma once

#include "..\Core\Core.h"
#include "..\Graphics\Graphics.h"
#include "..\Resource\Resource.h"

class Renderer abstract : public ComponentBase
{
public:
	typedef vector<shared_ptr<Material>> MaterialVec;

public:
	Renderer()
		:
		_materials(nullptr)
	{
	}

	virtual ~Renderer() {}

	virtual void Update(void* pData) abstract;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) abstract;
	virtual void Destroy() abstract;

	int GetMaterialCount() const;
	shared_ptr<Material> GetMaterial(int index) const;
	void SetMaterialCount(int count);
	void SetMaterial(int index, const shared_ptr<Material> material);

protected:
	shared_ptr<MaterialVec>	_materials;
};

inline int Renderer::GetMaterialCount() const
{
	return _materials->size();
}
inline shared_ptr<Material> Renderer::GetMaterial(int index) const
{
	if (index > _materials->size() - 1) return nullptr;

	return _materials->data()[index];
}

inline void Renderer::SetMaterialCount(int count)
{
	_materials->resize(count);
}

inline void Renderer::SetMaterial(int index, const shared_ptr<Material> material)
{
	if (index > _materials->size() - 1) return;

	_materials->data()[index] = material;
}

