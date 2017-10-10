#include "QuadTree.h"
#include "Frustum.h"

QuadTree::QuadTree(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight)
	:
	_pParent(parent),
	_radius(0),
	_culled(false)
{
	for (int i = 0; i < 4; i++)
	{
		_pChild[i] = nullptr;
		_pNeighbour[i] = nullptr;
	}

	setCorners(topLeft, topRight, bottomLeft, bottomRight);
}

QuadTree::QuadTree(int x, int z)
	: QuadTree(nullptr, 0, x - 1, x * (z - 1), x * z - 1)
{
}

QuadTree::~QuadTree()
{
	Destroy();
}
////////////////////////////////////////////////////////////////////////////////////////
//////////////// public method /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

bool QuadTree::Build(shared_ptr<HeightMapVec> pHeightMapVec)
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	buildQuadTree(pHeightMapVec);
	buildNeighbourNode(this, pHeightMapVec, _corner[1] + 1);
	return true;
}

int QuadTree::GenerateIndex(void* indices, shared_ptr<HeightMapVec> pHeightMapVec, shared_ptr<Frustum> pFrustum, float LODRatio)
{
	assert(indices && "null reference: indices");
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	assert(pFrustum && "null reference: pFrustum");

	processFrustumCulling(pHeightMapVec, pFrustum);
	return genrateTriangleIndex(0, indices, pHeightMapVec, pFrustum, LODRatio);
}

////////////////////////////////////////////////////////////////////////////////////////
//////////////// private method ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/// constructor
void QuadTree::setCorners(int topLeft, int topRight, int bottomLeft, int bottomRight)
{
	_corner[0] = topLeft;
	_corner[1] = topRight;
	_corner[2] = bottomLeft;
	_corner[3] = bottomRight;
	_center = (_corner[0] + _corner[1] + _corner[2] + _corner[3]) / 4;
}

/// destructor
void QuadTree::Destroy()
{
	SafeDelete<QuadTree>(_pChild[0]);
	SafeDelete<QuadTree>(_pChild[1]);
	SafeDelete<QuadTree>(_pChild[2]);
	SafeDelete<QuadTree>(_pChild[3]);
}

/// build
bool QuadTree::buildQuadTree(shared_ptr<HeightMapVec> pHeightMapVec)
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");

	if (subDivide())
	{
		// ������ܰ�, ���� �ϴ��� �Ÿ��� ���Ѵ�.
		Vector3 v = pHeightMapVec->data()[_corner[0]] - pHeightMapVec->data()[_corner[3]];
		// v�� �Ÿ����� �� ��带 ���δ� ��豸�� �����̹Ƿ�, 
		// 2�� ������ �������� ���Ѵ�.
		_radius = D3DXVec3Length(&v) / 2.0f;
		_pChild[0]->buildQuadTree(pHeightMapVec);
		_pChild[1]->buildQuadTree(pHeightMapVec);
		_pChild[2]->buildQuadTree(pHeightMapVec);
		_pChild[3]->buildQuadTree(pHeightMapVec);
	}
	return true;
}

void QuadTree::buildNeighbourNode(QuadTree* pRoot, shared_ptr<HeightMapVec> pHeightMapVec, int width)
{
	assert(pRoot && "null reference: pRoot");
	assert(pHeightMapVec && "null reference: pHeightMapVec");

	bool isNeighbourVaild;

	int topLeft;
	int topRight;
	int bottomLeft;
	int bottomRight;

	for (int i = 0; i < 4; i++)
	{
		// �̿������ 4�� �ڳʰ��� ��´�.
		isNeighbourVaild = getNeighbourCorners(i, width, topLeft, topRight, bottomLeft, bottomRight);
		// �ڳʰ����� �̿������ �����͸� ���´�.

		if (isNeighbourVaild)
		{
			_pNeighbour[i] = pRoot->findNode(pHeightMapVec, topLeft, topRight, bottomLeft, bottomRight);
		}
	}

	// �ڽĳ��� ���ȣ��
	if (_pChild[0])
	{
		_pChild[0]->buildNeighbourNode(pRoot, pHeightMapVec, width);
		_pChild[1]->buildNeighbourNode(pRoot, pHeightMapVec, width);
		_pChild[2]->buildNeighbourNode(pRoot, pHeightMapVec, width);
		_pChild[3]->buildNeighbourNode(pRoot, pHeightMapVec, width);
	}
}

///generateIndex
void QuadTree::processFrustumCulling(shared_ptr<HeightMapVec> pHeightMapVec, shared_ptr<Frustum> pFrustum)
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	assert(pFrustum && "null reference: pFrustum");

	int isFrustum = isInFrustum(pHeightMapVec, pFrustum);

	switch (isFrustum)
	{
	case eFrustumOut:				// �������ҿ��� �������, ������� �˻� �ʿ����
		_culled = true;
		return;
	case eFrustumIn:	// �������ҿ� ��������, ������� �˻� �ʿ����
		setInFrustumAll();
		return;
	case eFrustumPartiallyIn:		// �������ҿ� �Ϻ�����, ������� �˻� �ʿ���
		_culled = false;
		if (_pChild[0]) _pChild[0]->processFrustumCulling(pHeightMapVec, pFrustum);
		if (_pChild[1]) _pChild[1]->processFrustumCulling(pHeightMapVec, pFrustum);
		if (_pChild[2]) _pChild[2]->processFrustumCulling(pHeightMapVec, pFrustum);
		if (_pChild[3]) _pChild[3]->processFrustumCulling(pHeightMapVec, pFrustum);
	}
}



int QuadTree::genrateTriangleIndex(int numTriangles, void* indices, shared_ptr<HeightMapVec> pHeightMapVec, const shared_ptr<Frustum> pFrustum, float LODRatio)
{
	assert(indices && "null reference: indices");
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	assert(pFrustum && "null reference: pFrustum");

	if (_culled)
	{
		_culled = false;
		return numTriangles;
	}

	if (isVisible(pHeightMapVec, pFrustum->GetCameraPos(), LODRatio))
	{
		INDEX* p = ((INDEX*)indices) + numTriangles;
		INDEX index;

		if (_corner[1] - _corner[0] <= 1)
		{
			index._0 = _corner[0];
			index._1 = _corner[2];
			index._2 = _corner[1];
			
			*p++ = index;
			numTriangles++;

			index._0 = _corner[2];
			index._1 = _corner[3];
			index._2 = _corner[1];
			
			*p++ = index;
			numTriangles++;

			return numTriangles;
		}

		bool b[5] = { false, };
		
		if (_pNeighbour[eUp]) b[eUp] = _pNeighbour[eUp]->isVisible(pHeightMapVec, pFrustum->GetCameraPos(), LODRatio);
		if (_pNeighbour[eDown]) b[eDown] = _pNeighbour[eDown]->isVisible(pHeightMapVec, pFrustum->GetCameraPos(), LODRatio);
		if (_pNeighbour[eLeft]) b[eLeft] = _pNeighbour[eLeft]->isVisible(pHeightMapVec, pFrustum->GetCameraPos(), LODRatio);
		if (_pNeighbour[eRight]) b[eRight] = _pNeighbour[eRight]->isVisible(pHeightMapVec, pFrustum->GetCameraPos(), LODRatio);

		b[4] = b[eUp] & b[eDown] & b[eLeft] & b[eRight];		
		
		if (b[4])
		{	
			index._0 = _corner[0];
			index._1 = _corner[2];
			index._2 = _corner[1];
			
			*p++ = index;
			numTriangles++;

			index._0 = _corner[2];
			index._1 = _corner[3];
			index._2 = _corner[1];			
			*p++ = index;
			numTriangles++;
			return numTriangles;
		}

		int	n;

		if (!b[eUp] && _pNeighbour[eUp]) // ��� �κк����� �ʿ��Ѱ�?
		{			
			n = (_corner[2] + _corner[3]) / 2;
			index._0 = _center;
			index._1 = n;
			index._2 = _corner[3];
			*p++ = index;
			numTriangles++;

			index._0 = _center;
			index._1 = _corner[2];
			index._2 = n;			
			*p++ = index;
			numTriangles++;
		}
		else	// ��� �κк����� �ʿ���� ���
		{
			index._0 = _center;
			index._1 = _corner[2];
			index._2 = _corner[3];
			*p++ = index;
			numTriangles++;
		}

		if (!b[eDown] && _pNeighbour[eDown]) // �ϴ� �κк����� �ʿ��Ѱ�?
		{
			n = (_corner[0] + _corner[1]) / 2;
			index._0 = _center;
			index._1 = n;
			index._2 = _corner[0];
			*p++ = index;
			numTriangles++;
			index._0 = _center;
			index._1 = _corner[1];
			index._2 = n;			
			*p++ = index;
			numTriangles++;
		}
		else	// �ϴ� �κк����� �ʿ���� ���
		{
			index._0 = _center;
			index._1 = _corner[1];
			index._2 = _corner[0];
			*p++ = index;
			numTriangles++;
		}

		if (!b[eLeft] && _pNeighbour[eLeft]) // ���� �κк����� �ʿ��Ѱ�?
		{
			n = (_corner[0] + _corner[2]) / 2;
			index._0 = _center;
			index._1 = n;
			index._2 = _corner[2];			
			*p++ = index;
			numTriangles++;
			index._0 = _center;
			index._1 = _corner[0];
			index._2 = n;			
			*p++ = index;
			numTriangles++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{
			index._0 = _center;
			index._1 = _corner[0];
			index._2 = _corner[2];			
			*p++ = index;
			numTriangles++;
		}

		if (!b[eRight] && _pNeighbour[eRight]) // ���� �κк����� �ʿ��Ѱ�?
		{
			n = (_corner[1] + _corner[3]) / 2;
			index._0 = _center;
			index._1 = n;
			index._2 = _corner[1];			
			*p++ = index;
			numTriangles++;

			index._0 = _center;
			index._1 = _corner[3];
			index._2 = n;			
			*p++ = index;
			numTriangles++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{
			index._0 = _center;
			index._1 = _corner[3];
			index._2 = _corner[1];			
			*p++ = index;
			numTriangles++;
		}

		return numTriangles;	// �� ��� �Ʒ��� �ڽĳ��� Ž���� �ʿ�����Ƿ� ����!
	}

	if (_pChild[0]) numTriangles = _pChild[0]->genrateTriangleIndex(numTriangles, indices, pHeightMapVec, pFrustum, LODRatio);
	if (_pChild[1]) numTriangles = _pChild[1]->genrateTriangleIndex(numTriangles, indices, pHeightMapVec, pFrustum, LODRatio);
	if (_pChild[2]) numTriangles = _pChild[2]->genrateTriangleIndex(numTriangles, indices, pHeightMapVec, pFrustum, LODRatio);
	if (_pChild[3]) numTriangles = _pChild[3]->genrateTriangleIndex(numTriangles, indices, pHeightMapVec, pFrustum, LODRatio);

	return numTriangles;
}

/// buildQuadTree
bool QuadTree::subDivide()
{
	if ((_corner[1] - _corner[0]) <= 1) return false;

	int topEdgeCenter = (_corner[0] + _corner[1]) / 2;
	int bottomEdgeCenter = (_corner[2] + _corner[3]) / 2;
	int leftEdgeCenter = (_corner[0] + _corner[2]) / 2;
	int rightEdgeCenter = (_corner[1] + _corner[3]) / 2;
	int centerPoint = (_corner[0] + _corner[1] + _corner[2] + _corner[3]) / 4;

	_pChild[0] = CreateChild(this, _corner[0], topEdgeCenter, leftEdgeCenter, centerPoint);
	_pChild[1] = CreateChild(this, topEdgeCenter, _corner[1], centerPoint, rightEdgeCenter);
	_pChild[2] = CreateChild(this, leftEdgeCenter, centerPoint, _corner[2], bottomEdgeCenter);
	_pChild[3] = CreateChild(this, centerPoint, rightEdgeCenter, bottomEdgeCenter, _corner[3]);

	return true;
}

/// buildNeighbourNode
QuadTree* QuadTree::findNode(shared_ptr<HeightMapVec> pHeightMapVec, int topLeft, int topRight, int bottomLeft, int bottomRight)
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");

	if (_corner[0] == topLeft &&
		_corner[1] == topRight &&
		_corner[2] == bottomLeft &&
		_corner[3] == bottomRight)
	{
		return this;
	}
	

	if (_pChild[0])
	{
		RECT	rc;
		
		POINT	pt;
		int n = (topLeft + topRight + bottomLeft + bottomRight) / 4;

		// ���� �ʻ󿡼��� ��ġ
		pt.x = (int)pHeightMapVec->data()[n].x;
		pt.y = (int)pHeightMapVec->data()[n].z;

		for (int i = 0; i < 4; i++)
		{
			SetRect(&rc, (int)pHeightMapVec->data()[_pChild[i]->_corner[2]].x,
						(int)pHeightMapVec->data()[_pChild[i]->_corner[2]].z,
						(int)pHeightMapVec->data()[_pChild[i]->_corner[1]].x,
						(int)pHeightMapVec->data()[_pChild[i]->_corner[1]].z);
			// pt���� ���������ȿ� �ִٸ� �ڽĳ��� ����.
			if ((rc.left <= pt.x) && (pt.x <= rc.right) && (rc.bottom <= pt.y) && (pt.y <= rc.top))
				return _pChild[i]->findNode(pHeightMapVec, topLeft, topRight, bottomLeft, bottomRight);
		}
	}

	return nullptr;
}

bool QuadTree::getNeighbourCorners(int edgeType, int width, int& bottomLeft, int& bottomRight, int& topLeft, int& topRight)
{
	int edgeSize = _corner[1] - _corner[0];	// ���� ����� �¿�����

	switch (edgeType)
	{
	case eUp:	// ���� ���� �̿������ �ε���
		topLeft = _corner[2] + width * edgeSize;
		topRight = _corner[3] + width * edgeSize;
		bottomLeft = _corner[2];
		bottomRight = _corner[3];
		break;
	case eDown:	// �Ʒ� ���� �̿������ �ε���
		topLeft = _corner[0];
		topRight = _corner[1];
		bottomLeft = _corner[0] - width * edgeSize;
		bottomRight = _corner[1] - width * edgeSize;
		break;
	case eLeft:	// ���� ���� �̿������ �ε���
		topLeft = _corner[2] - edgeSize;
		topRight = _corner[2];
		bottomLeft = _corner[0] - edgeSize;
		bottomRight = _corner[0];
		break;
	case eRight:	// ���� ���� �̿������ �ε���
		topLeft = _corner[3];
		topRight = _corner[3] + edgeSize;
		bottomLeft = _corner[1];
		bottomRight = _corner[1] + edgeSize;
		break;
	}

	bool b[4] = { false, };
	b[0] = topRight > width * width;		// ���� �̿���尡 HeightMap ������ ���.
	b[1] = bottomLeft < 0;					// �Ʒ��� �̿���尡 HeightMap ������ ���.
	b[2] = topLeft % width == width - 1;	// ���� �̿���尡 HeightMap ������ ���.
	b[3] = topRight % width == 0;			// ������ �̿���尡 HeightMap ������ ���.

	if (b[0] | b[1] | b[2] | b[3])
		return false;

	return true;
}

/// processFrustumCulling
void	QuadTree::setInFrustumAll()
{
	_culled = false;
	if (!_pChild[0]) return;
	_pChild[0]->setInFrustumAll();
	_pChild[1]->setInFrustumAll();
	_pChild[2]->setInFrustumAll();
	_pChild[3]->setInFrustumAll();
}



int QuadTree::isInFrustum(shared_ptr<HeightMapVec> pHeightMapVec, shared_ptr<Frustum> pFrustum) const
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	assert(pFrustum && "null reference: pFrustum");

	if (!pFrustum->IsInSphere(pHeightMapVec->data()[_center], _radius)) return eFrustumOut;

	if (pFrustum->IsIn(pHeightMapVec->data()[_corner[0]]) &&
		pFrustum->IsIn(pHeightMapVec->data()[_corner[1]]) &&
		pFrustum->IsIn(pHeightMapVec->data()[_corner[2]]) &&
		pFrustum->IsIn(pHeightMapVec->data()[_corner[3]])) return eFrustumIn;

	return eFrustumPartiallyIn;
}




