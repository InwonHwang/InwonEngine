#include "NavigationMesh.h"
#include "Component.h"

// 인접 폴리곤 추가
void NavMeshPolygon::AddNeighbour(const shared_ptr<NavMeshPolygon> pNavMeshPolygon)
{
	_neighbourList.push_back(pNavMeshPolygon);
}

// 같은 위치의 버텍스가 있으면 인접해있음.
bool NavMeshPolygon::IsNeighbour(const shared_ptr<NavMeshPolygon> pPolygon) const
{
	int adjacencyCount = 0;

	for (int i = 0; i < 3; ++i)
	{
		if (this->_verticeVec[i] == pPolygon->_verticeVec[0] ||
			this->_verticeVec[i] == pPolygon->_verticeVec[1] ||
			this->_verticeVec[i] == pPolygon->_verticeVec[2])
			adjacencyCount++;
	}

	return adjacencyCount == 2 ? true : false;
}

// 해당 pos가 polygon 내에 있는지
bool NavMeshPolygon::Contains(const Vector3& pos, const Vector3& offset) const
{
	Vector3 p0 = GetVertex(0) + offset;
	Vector3 p1 = GetVertex(2) + offset;
	Vector3 p2 = GetVertex(1) + offset;

	float s = (p0.z * p2.x - p0.x * p2.z + (p2.z - p0.z) * pos.x + (p0.x - p2.x) * pos.z);
	float t = (p0.x * p1.z - p0.z * p1.x + (p0.z - p1.z) * pos.x + (p1.x - p0.x) * pos.z);

	if (s <= 0 || t <= 0) return false;

	float A = (-p1.z * p2.x + p0.z * (-p1.x + p2.x) + p0.x * (p1.z - p2.z) + p1.x * p2.z);

	return (s + t) < A;
}

// 해당 pos로부터 가장 가까이 있는 vertex 리턴
Vector3 NavMeshPolygon::GetClosestVertex(const Vector3& pos) const
{
	float closestDist = Mathf::Infinity();
	Vector3 ret;

	for (int i = 0; i < 3; ++i)
	{
		float dist = Vector3::GetDistance(_verticeVec[i], pos);
		if (dist < closestDist)
		{
			closestDist = dist;
			ret = _verticeVec[i];
		}
	}

	return ret;
}

NavMeshPolygon::State NavMeshPolygon::ContainedFromObtacle(shared_ptr<GameObject> gameObject, const Vector3& offset)
{
	shared_ptr<MeshRenderer> pRenderer = gameObject->GetComponent<MeshRenderer>();

	if (!pRenderer) return eObtacleOut;

	Vector3 boxSize;// = pRenderer->GetMesh()->GetBoxSize();
	Vector3 objectPos, objectScale;
	Vector3 v1, v2, v3;

	gameObject->GetComponent<Transform>()->GetPosition(objectPos);
	gameObject->GetComponent<Transform>()->GetScale(objectScale);
	boxSize.x *= objectScale.x;
	boxSize.y *= objectScale.y;
	boxSize.z *= objectScale.z;

	v1 = GetVertex(0) + offset;
	v2 = GetVertex(1) + offset;
	v3 = GetVertex(2) + offset;

	if ((v1.y < objectPos.y - boxSize.y &&
		v2.y < objectPos.y - boxSize.y &&
		v3.y < objectPos.y - boxSize.y))
		return eObtacleOut;

	if ((v1.y > objectPos.y + boxSize.y &&
		v2.y > objectPos.y + boxSize.y &&
		v3.y > objectPos.y + boxSize.y))
		return eObtacleOut;

	Vector3 c1, c2, c3, c4;

	c1.x = objectPos.x + boxSize.x;
	c1.z = objectPos.z + boxSize.z;
	c2.x = objectPos.x - boxSize.x;
	c2.z = objectPos.z - boxSize.z;
	c3.x = objectPos.x + boxSize.x;
	c3.z = objectPos.z - boxSize.z;
	c4.x = objectPos.x - boxSize.x;
	c4.z = objectPos.z + boxSize.z;

	if (Contains(c1, offset) ||
		Contains(c2, offset) ||
		Contains(c3, offset) ||
		Contains(c4, offset))
	{
		return eObtaclePartiallyIn;
	}

	if ((v1.x < objectPos.x - boxSize.x || v1.z < objectPos.z - boxSize.z ||
		v1.x > objectPos.x + boxSize.x || v1.z > objectPos.z + boxSize.z) &&
		(v2.x < objectPos.x - boxSize.x || v2.z < objectPos.z - boxSize.z ||
			v2.x > objectPos.x + boxSize.x || v2.z > objectPos.z + boxSize.z) &&
			(v3.x < objectPos.x - boxSize.x || v3.z < objectPos.z - boxSize.z ||
				v3.x > objectPos.x + boxSize.x || v3.z > objectPos.z + boxSize.z))
	{
		return eObtacleOut;
	}



	if (v1.x < objectPos.x + boxSize.x && v1.x > objectPos.x - boxSize.x &&
		v1.z < objectPos.z + boxSize.z && v1.z > objectPos.z - boxSize.z &&
		v2.x < objectPos.x + boxSize.x && v2.x > objectPos.x - boxSize.x &&
		v2.z < objectPos.z + boxSize.z && v2.z > objectPos.z - boxSize.z &&
		v3.x < objectPos.x + boxSize.x && v3.x > objectPos.x - boxSize.x &&
		v3.z < objectPos.z + boxSize.z && v3.z > objectPos.z - boxSize.z)
		return eObtacleIn;

	return eObtaclePartiallyIn;
}

Vector3 NavMeshPolygon::GetCenter() const
{
	return _center;
}

Vector3 NavMeshPolygon::GetVertex(int i) const
{
	return _verticeVec[i];
}

void NavMeshGraph::DebugDraw(IDirect3DDevice9* pd3dDevice)
{
	vector<WORD> indiceVec;
	vector<Vector3> verticeVec;

	int c = 0;
	for (int i = 0; i < _polygonVec.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			verticeVec.push_back(_polygonVec[i]->GetVertex(j));
			indiceVec.push_back(c++);
		}
	}

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	pd3dDevice->SetFVF(D3DFVF_XYZ);
	pd3dDevice->SetStreamSource(0, NULL, 0, sizeof(Vector3));
	pd3dDevice->SetTexture(0, NULL);
	pd3dDevice->SetIndices(0);

	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	pd3dDevice->SetMaterial(&mtrl);
	pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, verticeVec.size(),
		_polygonVec.size(), indiceVec.data(), D3DFMT_INDEX16, verticeVec.data(), sizeof(Vector3));
}

void NavMeshGraph::BuildGraph(const list<weak_ptr<GameObject>>& obtacleList,
	const vector<Vector3>& verticeVec,
	const vector<INDEX>& indiceVec,
	const Vector3& offset,
	int tessellateLevel)
{
	list<shared_ptr<NavMeshPolygon>> polygonList;
	list<shared_ptr<NavMeshPolygon>> tessellatedList;
	list<shared_ptr<NavMeshPolygon>> toBeRemovedList;

	for (size_t i = 0; i < indiceVec.size(); ++i)
	{
		int index0 = indiceVec[i]._0;
		int index1 = indiceVec[i]._1;
		int index2 = indiceVec[i]._2;

		shared_ptr<NavMeshPolygon> pNavMeshPolygon(new NavMeshPolygon(verticeVec[index0], verticeVec[index1], verticeVec[index2]));

		//Polygon의 법선벡터의 방향이 (0,1,0)벡터와 90도 이상 차이나는지; 		
		float result = calculateSlope(pNavMeshPolygon);
		if (result >= 0.3f)
			polygonList.push_back(pNavMeshPolygon);
	}

	for (auto pPolygon : polygonList)
	{
		for (auto pObtacle : obtacleList)
		{
			NavMeshPolygon::State state = pPolygon->ContainedFromObtacle(pObtacle.lock(), offset);

			if (state == NavMeshPolygon::eObtacleOut) continue;

			toBeRemovedList.push_back(pPolygon);
		}
	}

	for (auto pToBeRemoved : toBeRemovedList)
	{
		polygonList.remove(pToBeRemoved);
	}

	// 이과정을 반복
	for (int i = 0; i < 1; i++)
	{
		tessellate(toBeRemovedList, tessellatedList);
		toBeRemovedList.clear();

		for (auto pPolygon : tessellatedList)
		{
			for (auto pObtacle : obtacleList)
			{
				NavMeshPolygon::State state = pPolygon->ContainedFromObtacle(pObtacle.lock(), offset);

				if (state == NavMeshPolygon::eObtacleOut) continue;

				toBeRemovedList.push_back(pPolygon);
			}
		}

		for (auto pToBeRemoved : toBeRemovedList)
		{
			tessellatedList.remove(pToBeRemoved);
		}

		for (auto pPolygon : tessellatedList)
		{
			polygonList.push_back(pPolygon);
		}
	}


	_polygonVec.assign(polygonList.begin(), polygonList.end());

	buildNeighbour();
}

shared_ptr<NavMeshPolygon> NavMeshGraph::GetClosestPolygon(const Vector3& pos) const
{
	float minDist = Mathf::Infinity();
	float dist;
	shared_ptr<NavMeshPolygon> pPolygon;

	for (int i = 0; i < _polygonVec.size(); ++i)
	{
		dist = Vector3::GetDistance(pos, _polygonVec[i]->GetCenter());
		
		if (dist < minDist)
		{
			minDist = dist;
			pPolygon = _polygonVec[i];
		}
	}

	return pPolygon;
}

void NavMeshGraph::buildNeighbour()
{
	for (size_t i = 0; i < _polygonVec.size() - 1; ++i)
	{
		for (size_t j = 0; j < _polygonVec.size(); ++j)
		{
			if (_polygonVec[i]->IsNeighbour(_polygonVec[j]) && i != j)
			{
				_polygonVec[i]->AddNeighbour(_polygonVec[j]);
			}
		}
	}
}

void NavMeshGraph::tessellate(const list<shared_ptr<NavMeshPolygon>>& polygonList, list<shared_ptr<NavMeshPolygon>>& newList)
{
	for (auto pPolygon : polygonList)
	{
		tessellatePolygon(newList, pPolygon);
	}
}

void NavMeshGraph::tessellatePolygon(list<shared_ptr<NavMeshPolygon>>& newList, const shared_ptr<NavMeshPolygon> pPolygon)
{
	shared_ptr<NavMeshPolygon> p1, p2, p3, p4;
	Vector3 v1, v2, v3;
	Vector3 v4, v5, v6;

	v1 = pPolygon->GetVertex(0);
	v2 = pPolygon->GetVertex(1);
	v3 = pPolygon->GetVertex(2);

	v4 = (v1 + v2) / 2;
	v5 = (v2 + v3) / 2;
	v6 = (v1 + v3) / 2;

	p1.reset(new NavMeshPolygon(v1, v4, v6));
	p2.reset(new NavMeshPolygon(v4, v5, v6));
	p3.reset(new NavMeshPolygon(v4, v2, v5));
	p4.reset(new NavMeshPolygon(v6, v5, v3));

	newList.push_back(p1);
	newList.push_back(p2);
	newList.push_back(p3);
	newList.push_back(p4);
}

float NavMeshGraph::Intersect(const RAY& ray, POINTF& point, const Vector3& offset)
{
	size_t i = 0;
	float d = -1;

	while (i < _polygonVec.size() && d == -1)
	{

		Vector3 v1, v2, v3;
		v1 = _polygonVec[i]->GetVertex(0) + offset;
		v2 = _polygonVec[i]->GetVertex(1) + offset;
		v3 = _polygonVec[i]->GetVertex(2) + offset;

		d = intersectTri(ray, point, v1, v2, v3);

		++i;
	}

	return d;
}

float NavMeshGraph::intersectTri(const RAY& ray, POINTF& point, const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	float d = 0;
	bool f = D3DXIntersectTri(&v1, &v2, &v3, &ray.origin, &ray.direction, &point.x, &point.y, &d);

	if (f) return d;

	return -1;
}

float NavMeshGraph::calculateSlope(const shared_ptr<NavMeshPolygon> pPolygon) const
{
	// 법선 벡터 구하기.
	Vector3 e1 = (pPolygon->GetVertex(0) - pPolygon->GetVertex(1));
	Vector3 e2 = (pPolygon->GetVertex(0) - pPolygon->GetVertex(2));
	D3DXVec3Normalize(&e1, &e1);
	D3DXVec3Normalize(&e2, &e2);

	Vector3 n;
	D3DXVec3Cross(&n, &e1, &e2);
	D3DXVec3Normalize(&n, &n);

	return D3DXVec3Dot(&n, &Vector3(0, 1, 0));
}

void NavigationMesh::Update(void* pData)
{
}

void NavigationMesh::Create(void* pData, const shared_ptr<void> gameObject)
{
	_gameObject = static_pointer_cast<GameObject>(gameObject);
	_navMeshGraph.reset(new NavMeshGraph);
}

void NavigationMesh::Destroy()
{
	_gameObject.reset();
	_navMeshGraph.reset();
}

void NavigationMesh::DebugDraw(IDirect3DDevice9* pd3dDevice)
{
	_navMeshGraph->DebugDraw(pd3dDevice);
}

float NavigationMesh::Intersect(const RAY& ray, POINTF& point)
{
	Vector3 offset;
	_gameObject->GetComponent<Transform>()->GetPosition(offset);
	return _navMeshGraph->Intersect(ray, point, offset);
}

void NavigationMesh::Bake()
{
	shared_ptr<MeshRenderer> pRenderer = _gameObject->GetComponent<MeshRenderer>();

	if (!pRenderer) return;

	Vector3 offset;
	_gameObject->GetComponent<Transform>()->GetPosition(offset);

	/*_navMeshGraph->BuildGraph(_obtacles,
		*pRenderer->GetMesh()->GetVertices(),
		*pRenderer->GetMesh()->GetIndice(),
		offset, 2);*/
}

int NavigationMesh::GetPath(const Vector3& begin, const Vector3& end) const
{
	priority_queue<shared_ptr<NavMeshPolygon>, vector<shared_ptr<NavMeshPolygon>>, CompareNavMeshPolygon> pq;
	list<shared_ptr<NavMeshPolygon>> _openList;
	list<shared_ptr<NavMeshPolygon>> _closeList;
	shared_ptr<NavMeshPolygon> cur;

	shared_ptr<NavMeshPolygon> start = _navMeshGraph->GetClosestPolygon(begin);
	shared_ptr<NavMeshPolygon> finish = _navMeshGraph->GetClosestPolygon(end);

	cur = start;

	float cost = 0;
	float heuristic = Vector3::GetDistance(cur->GetCenter(), finish->GetCenter());

	cur->SetCost(cost);
	cur->SetHeuristic(heuristic);

	pq.push(cur);
	_openList.push_back(cur);

	while (!pq.empty())
	{
		cur = pq.top();
		pq.pop();

		_openList.remove(cur);
		_closeList.push_back(cur);

		for (int i = 0; i < cur->GetNeighbourCount(); ++i)
		{
			shared_ptr<NavMeshPolygon> pNeighbour = cur->GetNeighbour(i);
			auto it1 = find(_closeList.begin(), _closeList.end(), pNeighbour);
			auto it2 = find(_openList.begin(), _openList.end(), pNeighbour);

			if (it1 == _closeList.end() && it2 == _openList.end())
			{
				cost = cur->GetCost() + Vector3::GetDistance(pNeighbour->GetCenter(), cur->GetCenter());
				heuristic = Vector3::GetDistance(pNeighbour->GetCenter(), finish->GetCenter());
				pNeighbour->SetCost(cost);
				pNeighbour->SetHeuristic(heuristic);
				pNeighbour->SetParent(cur);

				pq.push(pNeighbour);
				_openList.push_back(pNeighbour);
			}
		}
	}

	shared_ptr<NavMeshPolygon> temp = finish->GetParent();
	int t = 0;
	while (temp)
	{
		t++;
		temp = temp->GetParent();
	}
	_navMeshGraph->Reset();

	return t;
}


