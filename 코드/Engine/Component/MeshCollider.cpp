#include "MeshCollider.h"
#include "RigidBody.h"
#include "MeshRenderer.h"
#include "Transform.h"

MeshCollider::MeshCollider(const shared_ptr<GameObject> gameObject)
	: Collider(gameObject)
{
}

MeshCollider::~MeshCollider()
{
}

void MeshCollider::Update(void* pData)
{
	Collider::Update(pData);
}

void MeshCollider::Create(void* pData)
{
	auto meshRenderer = GetComponent<MeshRenderer>();
	createBulletMesh(meshRenderer->GetMesh());
	//_shape.reset(new btBvhTriangleMeshShape(_bulletMesh.get(), true, true));
}

void MeshCollider::Destroy()
{
	_bulletMesh.reset();
}

void MeshCollider::DebugDraw(IDirect3DDevice9* pd3dDevice)
{
	/*shared_ptr<btConvexTriangleMeshShape> pConvexShape = std::dynamic_pointer_cast<btConvexTriangleMeshShape>(_shape);	
	shared_ptr<vector<Vector3>> pVerticeVec;
	shared_ptr<vector<INDEX>> pIndiceVec;
	vector<BOXVERTEX> vvec;

	if (pConvexShape)
	{
		int verticeCount = pConvexShape->getNumVertices();
		btVector3 pos;
		BOXVERTEX vtx;

		vvec.resize(verticeCount);

		for (int i = 0; i < verticeCount; ++i)
		{
			pConvexShape->getVertex(i, pos);
			vtx.x = float(pos.getX());
			vtx.y = float(pos.getY());
			vtx.x = float(pos.getZ());
			vtx.color = 0xffffffff;

			vvec[i] = vtx;
		}

		return;
	}
	else
	{
		pVerticeVec = _gameObject->GetComponent<MeshRenderer>()->GetMesh()->GetSubMesh(0)->GetVertices();
		pIndiceVec = _gameObject->GetComponent<MeshRenderer>()->GetMesh()->GetSubMesh(0)->GetIndices();

		shared_ptr<StaticMesh> pMesh = _gameObject->GetComponent<MeshRenderer>()->GetMesh()->GetSubMesh(0);

		vvec.resize(pVerticeVec->size());

		for (int i = 0; i < pVerticeVec->size(); ++i)
		{
			vvec[i].x = pVerticeVec->data()[i].x;
			vvec[i].y = pVerticeVec->data()[i].y;
			vvec[i].z = pVerticeVec->data()[i].z;
			vvec[i].color = 0xffffffff;
		}
	}

	Vector3 t;
	Quaternion q;
	D3DXMATRIX r, tm, qm;
	_gameObject->GetComponent<Transform>()->GetPosition(t);
	_gameObject->GetComponent<Transform>()->GetRotation(q);
	D3DXMatrixTranslation(&tm, t.x, t.y + 0.1f, t.z);
	D3DXMatrixRotationQuaternion(&qm, &q);

	r = qm * tm;

	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	pd3dDevice->SetTransform(D3DTS_WORLD, &r);
	pd3dDevice->SetFVF(BOXVERTEX::FVF);
	pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, vvec.size(), pIndiceVec->size(),
		pIndiceVec->data(), D3DFMT_INDEX32, vvec.data(), sizeof(BOXVERTEX));

	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/
}

void MeshCollider::createBulletMesh(const shared_ptr<Mesh> pMeshContainer)
{
	size_t subMeshCount = pMeshContainer->GetSubMeshCount();

	shared_ptr<vector<Vector3>> pVerticeVec;
	shared_ptr<vector<INDEX>> pIndiceVec;

	for (int i = 0; i < subMeshCount; ++i)
	{
		pVerticeVec = pMeshContainer->GetSubMesh(i)->GetVertices();
		pIndiceVec = pMeshContainer->GetSubMesh(i)->GetIndices();
		size_t indiceCount = pMeshContainer->GetSubMesh(i)->GetIndices()->size();
		size_t idx1, idx2, idx3;

		for (int j = 0; j < indiceCount; ++j)
		{
			idx1 = pIndiceVec->data()[j]._0;
			idx2 = pIndiceVec->data()[j]._1;
			idx3 = pIndiceVec->data()[j]._2;

			btVector3 v1(btScalar(pVerticeVec->data()[idx1].x),
				btScalar(pVerticeVec->data()[idx1].y),
				btScalar(pVerticeVec->data()[idx1].z));

			btVector3 v2(btScalar(pVerticeVec->data()[idx2].x),
				btScalar(pVerticeVec->data()[idx2].y),
				btScalar(pVerticeVec->data()[idx2].z));

			btVector3 v3(btScalar(pVerticeVec->data()[idx3].x),
				btScalar(pVerticeVec->data()[idx3].y),
				btScalar(pVerticeVec->data()[idx3].z));

			_bulletMesh->addTriangle(v1, v2, v3, true);
		}
	}
}
