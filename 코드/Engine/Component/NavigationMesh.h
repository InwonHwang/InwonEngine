//#pragma once
//
////http://jceipek.com/Olin-Coding-Tutorials/pathing.html navmesh �Ұ�
////http://jsfiddle.net/dog_funtom/H7D7g/ �ﰢ�� ���� ã��
//
//#include "..\Core\Core.h"
//#include "..\Resource\Resource.h"
//
//class NavMeshPolygon
//{
//public:
//	enum State
//	{
//		eObtacleIn,
//		eObtaclePartiallyIn,
//		eObtacleOut
//	};
//public:
//	NavMeshPolygon(const Vector3& v1, const Vector3& v2, const Vector3& v3)
//		:
//		_heuristic(0),
//		_totalCost(0)
//	{
//		_verticeVec.resize(3);
//		_verticeVec[0] = v1;
//		_verticeVec[1] = v2;
//		_verticeVec[2] = v3;
//
//		_center = (v1 + v2 + v3) / 3;
//	}
//	~NavMeshPolygon() {}
//public:
//
//	// ���� ������ �߰�
//	void AddNeighbour(const shared_ptr<NavMeshPolygon> pNavMeshPolygon);
//
//	// ���� ��ġ�� ���ؽ��� ������ ����������.
//	bool IsNeighbour(const shared_ptr<NavMeshPolygon> pPolygon) const;
//
//	// �ش� pos�� polygon ���� �ִ���
//	bool Contains(const Vector3& pos, const Vector3& offset) const;
//
//	// �ش� pos�κ��� ���� ������ �ִ� vertex ����
//	Vector3 GetClosestVertex(const Vector3& pos) const;
//
//	// �ش� ������Ʈ�� ���� ���ԵǾ��ִ���
//	State ContainedFromObtacle(shared_ptr<GameObject> gameObject, const Vector3& offset);
//
//	Vector3 GetCenter() const;
//	Vector3 GetVertex(int i) const;
//	size_t GetNeighbourCount() const { return _neighbourList.size(); }
//	shared_ptr<NavMeshPolygon> GetNeighbour(int index) const
//	{
//		int i = 0;
//		auto it = _neighbourList.begin();
//		while (i != index)
//		{
//			it++;
//			i++;
//		}
//
//		return it->lock();
//	}
//
//	void SetHeuristic(float h) { _heuristic = h; }
//	void SetCost(float c) { _cost = c; }
//	float GetTotalCost() const { return _cost + _heuristic; }
//	float GetCost() const { return _cost; }
//	float GetHeuristic() const { return _heuristic; }
//
//	void SetParent(const shared_ptr<NavMeshPolygon> pParent) { _parent = pParent; }
//	shared_ptr<NavMeshPolygon> GetParent() const { return _parent.lock(); }
//
//private:
//	Vector3 _center;
//	vector<Vector3> _verticeVec;
//	list<weak_ptr<NavMeshPolygon>> _neighbourList;
//
//	weak_ptr<NavMeshPolygon> _parent;
//	float _heuristic;
//	float _cost;
//	float _totalCost;
//};
//
//class NavMeshGraph
//{
//public:
//	NavMeshGraph(int indiceCount)
//	{
//		_polygonVec.resize(indiceCount);
//	}
//	NavMeshGraph() {}
//	~NavMeshGraph() {}
//	//test
//	size_t getsize() { return _polygonVec.size(); }
//	void DebugDraw(IDirect3DDevice9* pd3dDevice);
//
//	float Intersect(const RAY& ray, POINTF& point, const Vector3& offset);
//
//	// ���� ��ġ���� ���� ����� ������ ����.
//	shared_ptr<NavMeshPolygon> GetClosestPolygon(const Vector3& pos) const;
//
//	void BuildGraph(const list<weak_ptr<GameObject>>& _obtacles,
//		const vector<Vector3>& verticeVec,
//		const vector<INDEX>& indiceVec,
//		const Vector3& offset,
//		int tessellateLevel = 0);
//
//	void Reset()
//	{
//		for (auto pPolygon : _polygonVec)
//		{
//			pPolygon->SetCost(0);
//			pPolygon->SetHeuristic(0);
//			pPolygon->SetParent(nullptr);
//		}
//
//	}
//private:
//	// �̿� ��� �߰�
//	void buildNeighbour();
//
//	// StaticMesh ����ȭ
//	void tessellate(const list<shared_ptr<NavMeshPolygon>>& polygonList, list<shared_ptr<NavMeshPolygon>>& newList);
//	void tessellatePolygon(list<shared_ptr<NavMeshPolygon>>& newList, const shared_ptr<NavMeshPolygon> pPolygon);
//
//	// up ���Ϳ� ���� ���.
//	float calculateSlope(const shared_ptr<NavMeshPolygon> pPolygon) const;
//
//	float intersectTri(const RAY& ray, POINTF& point, const Vector3& v1, const Vector3& v2, const Vector3& v3);
//
//private:
//	vector<shared_ptr<NavMeshPolygon>> _polygonVec;
//};
//
//struct CompareNavMeshPolygon
//{
//	bool operator()(const shared_ptr<NavMeshPolygon> lhs, const shared_ptr<NavMeshPolygon> rhs) const
//	{
//		return lhs->GetTotalCost() > rhs->GetTotalCost();
//	}
//};
//
//class NavigationMesh : public ComponentBase//, public std::enable_shared_from_this<NavigationMesh>
//{
//public:
//	NavigationMesh(const shared_ptr<GameObject> gameObject)
//		:
//		ComponentBase(gameObject),
//		_navMeshGraph(nullptr)
//	{
//	}
//	~NavigationMesh() {}
//
//	virtual void Update(void* pData) override;
//	virtual void Create(void* pData) override;
//	virtual void Destroy() override;
//
//	void DebugDraw(IDirect3DDevice9* pd3dDevice);
//	float Intersect(const RAY& ray, POINTF& point);
//	void Bake();
//	//shared_ptr<vector<Vector3>> GetPath(const Vector3& start, const Vector3& end) const;
//	int GetPath(const Vector3& start, const Vector3& end) const;
//
//	void AddObtacle(shared_ptr<GameObject> gameObject)
//	{
//		_obtacles.push_back(gameObject);
//	}
//
//private:
//	shared_ptr<NavMeshGraph> _navMeshGraph;
//	list<weak_ptr<GameObject>> _obtacles;
//};
//
