#include "Resource\Resource.h"
#include "Component\Component.h"
#include "Assets\script\CameraMove.h"
#include "Singleton\Singleton.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	if (Application::Instance()->Init(hInstance, nCmdShow))
	{
		// camera
		shared_ptr<GameObject> mainCamera = GameObject::Create();
		mainCamera->AddComponent<Transform>();
		mainCamera->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 0, 250));
		mainCamera->GetComponent<Transform>()->SetLocalRotation(Quaternion::Euler(0, 180, 0));
		mainCamera->AddComponent<CameraMove>();
		Camera::mainCamera = mainCamera->AddComponent<Camera>();

		// light
		//shared_ptr<GameObject> light = GameObject::Create();
		//light->AddComponent<Light>();
		//light->GetComponent<Transform>()->SetLocalRotation(Quaternion::Euler(-45.0f, 0.0f, 0.0f));

		//shared_ptr<FBXParser> pFbxImporter(new FBXParser());
		//pFbxImporter->ParseAndExport(string("Assets\\fbx\\Tree_Palm_01.fbx"));

		//auto treeMesh = Resources->Create<Mesh>("Assets\\mesh\\Tree_Palm_01.mesh");
		//auto treeMaterial = Resources->Create<Material>("Assets\\material\\default.material");

		////for (int i = 0; i < 5; ++i)
		//{
		//	auto tree = GameObject::Create();
		//	tree->AddComponent("MeshRenderer");
		//	tree->GetComponent<MeshRenderer>()->SetMesh(treeMesh);
		//	tree->GetComponent<MeshRenderer>()->SetMaterialCount(1);
		//	tree->GetComponent<MeshRenderer>()->SetMaterial(0, treeMaterial);
		//	//tree->GetComponent<Transform>()->SetLocalPosition(Vector3(i, 0, 0));
		//}

		/*auto knightMesh = Resources->Create<Mesh>("Assets\\mesh\\Knight.mesh");
		auto bouguMaterial = Resources->Create<Material>("Assets\\material\\Bougu.material");
		auto faceMaterial = Resources->Create<Material>("Assets\\material\\Face.material");

		auto knight = GameObject::Create();
		knight->AddComponent<SkinnedMeshRenderer>();
		knight->GetComponent<SkinnedMeshRenderer>()->SetMesh(knightMesh);
		knight->GetComponent<SkinnedMeshRenderer>()->SetMaterialCount(2);
		knight->GetComponent<SkinnedMeshRenderer>()->SetMaterial(0, bouguMaterial);
		knight->GetComponent<SkinnedMeshRenderer>()->SetMaterial(1, faceMaterial);*/

		auto prefab = Resources->Create<GameObject>("Assets\\prefab\\Tree_Palm_01.prefab");

		/*shared_ptr<GameObject> ball = GameObject::Instantiate(pModelData);
		ball->GetComponent<Transform>()->SetLocalScale(Vector3(0.05f, 0.05f, 0.05f));
		ball->AddComponent<RigidBody>();*/

		//// terrain
		/*shared_ptr<Effect> effect = ResourceMgr->Create<Effect>("assets\\effect\\DefaultTerrain.fx");
		shared_ptr<Material> material = ResourceMgr->Create<Material>("DefaultTerrain", nullptr);
		shared_ptr<Texture> textureTerrain = ResourceMgr->Create<Texture>("assets\\texture\\terrain.dds");
		shared_ptr<Texture> textureDetail = ResourceMgr->Create<Texture>("assets\\texture\\terrain_detail.dds");
		shared_ptr<TerrainData> terrainData = ResourceMgr->Create<TerrainData>("assets\\terrain\\heightMap.raw");

		material->SetEffect(effect);
		material->SetParamTexture("diffuseMap", textureTerrain);
		material->SetParamTexture("detailMap", textureDetail);

		shared_ptr<GameObject> terrain = GameObject::Create();
		terrain->AddComponent<Terrain>();
		terrain->GetComponent<Terrain>()->SetTerrainData(terrainData);
		terrain->GetComponent<Terrain>()->SetMaterialCount(1);
		terrain->GetComponent<Terrain>()->SetMaterial(0, material);*/
		// end terrain

		Application::Instance()->Update();
	}

	Application::Instance()->Release();

	return 0;
}