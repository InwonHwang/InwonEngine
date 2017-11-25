#pragma once

#include "..\Core\Core.h"
#include "..\Graphics\Graphics.h"
#include "..\Resource\Resource.h"

class Renderer abstract : public ComponentBase
{
	typedef vector<shared_ptr<Material>> MaterialVec;

protected:
	unique_ptr<MaterialVec>	_materials;

public:
	Renderer(const shared_ptr<GameObject> gameObject);
	virtual ~Renderer();

	virtual void Update(void* pData) abstract;
	virtual void Create(void* pData) override;
	virtual void Destroy() abstract;

	void SetMaterialCount(int count) { _materials->resize(count); }
	int GetMaterialCount() const { return _materials->size(); }

	void SetMaterial(int index, const shared_ptr<Material> material) { _materials->data()[index] = material; }
	const shared_ptr<Material> GetMaterial(int index) const { return _materials->data()[index]; }
};

