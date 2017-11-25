#pragma once

#include "..\Core\Core.h"

class SkyBox;

class Scene : public ResourceBase, public IExportable
{
private :
	shared_ptr<GameObject> _root;
	shared_ptr<SkyBox> _skyBox;

public:
	Scene();
	virtual ~Scene();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	virtual void ToJson(Json::Value& json) override;
	virtual void FromJson(const Json::Value& json) override;
	virtual void Export(const string& path) override;

	void SetSkyBox(const shared_ptr<SkyBox> skyBox) { _skyBox = skyBox; }
	shared_ptr<SkyBox> GetSkyBox() const { return _skyBox; }
	shared_ptr<GameObject> GetRootGameObject() const { return _root; }
	shared_ptr<GameObject> CreateGameObject() const;
};

