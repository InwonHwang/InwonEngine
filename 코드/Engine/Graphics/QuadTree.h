#pragma once
#include "..\Core\Core.h"

class Frustum;

class QuadTree
{
	typedef std::vector<Vector3> HeightMapVec;

	enum Corner {
		eTopLeft,
		eTopRight,
		eBottomLeft,
		eBottomRight
	};

	enum Edge {
		eUp,		// À­º¯
		eDown,		// ¾Æ·§º¯
		eLeft,		// ¿ÞÂÊ
		eRight		// ¿À¸¥ÂÊ
	};

	enum State {
		eFrustumOut,
		eFrustumPartiallyIn,
		eFrustumIn
	};
public:
	QuadTree(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight);
	QuadTree(int width, int cy);
	~QuadTree();


	bool Build(shared_ptr<HeightMapVec> pHeightMapVec);
	int GenerateIndex(void* pIndices, shared_ptr<HeightMapVec> pHeightMapVec, shared_ptr<Frustum> pFrustum, float LODRatio);

private:
	// constructor
	void		setCorners(int topLeft, int topRight, int bottomLeft, int bottomRight);

	// destructor
	void		Destroy();

	// build
	bool		buildQuadTree(shared_ptr<HeightMapVec> pHeightMapVec);
	void		buildNeighbourNode(QuadTree* pRoot, shared_ptr<HeightMapVec> pHeightMap, int width);

	// generateIndex
	void		processFrustumCulling(shared_ptr<HeightMapVec> pHeightMapVec, shared_ptr<Frustum> pFrustum);
	int			genrateTriangleIndex(int numTriangles, void* pIndices, shared_ptr<HeightMapVec> pHeightMapVec, const shared_ptr<Frustum> pFrustum, float LODRatio);

	// buildQuadTree
	bool		subDivide();

	// buildNeighbourNode
	QuadTree*	findNode(shared_ptr<HeightMapVec> pHeightMapVec, int topLeft, int topRight, int bottomLeft, int bottomRight);
	bool		getNeighbourCorners(int edgeType, int width, int& topLeft, int& topRight, int& bottomLeft, int& bottomRight);

	// processFrustumCulling
	int			isInFrustum(shared_ptr<HeightMapVec> pHeightMapVec, shared_ptr<Frustum> pFrustum)  const;
	void		setInFrustumAll();

	// inline	
	QuadTree*	CreateChild(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight) const;
	int			getLODLevel(shared_ptr<HeightMapVec> pHeightMapVec, const Vector3& cameraPos, float LODRatio) const;
	void		getCorners(int& topLeft, int& topRight, int& bottomLeft, int& bottomRight) const;
	bool		isVisible(shared_ptr<HeightMapVec> pHeightMapVec, const Vector3& cameraPos, float LODRatio) const;

private:
	QuadTree*	_pChild[4];
	QuadTree*	_pParent;
	QuadTree*	_pNeighbour[4];

	int			_corner[4];
	int			_center;
	bool		_culled;
	float		_radius;
};

inline QuadTree* QuadTree::CreateChild(QuadTree* parent, int topLeft, int topRight, int bottomLeft, int bottomRight) const
{
	return new QuadTree(parent, topLeft, topRight, bottomLeft, bottomRight);
}

inline int QuadTree::getLODLevel(const shared_ptr<HeightMapVec> pHeightMapVec, const Vector3& cameraPos, float LODRatio) const
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	float d = Vector3::GetDistance(pHeightMapVec->data()[_center], cameraPos);
	return max((int)(d * LODRatio), 1);
}

inline bool QuadTree::isVisible(const shared_ptr<HeightMapVec> pHeightMapVec, const Vector3& cameraPos, float LODRatio) const
{
	assert(pHeightMapVec && "null reference: pHeightMapVec");
	return (_corner[eTopRight] - _corner[eTopLeft] <= getLODLevel(pHeightMapVec, cameraPos, LODRatio));
}

inline void QuadTree::getCorners(int& topLeft, int& topRight, int& bottomLeft, int& bottomRight) const
{
	topLeft = _corner[0];
	topRight = _corner[1];
	bottomLeft = _corner[2];
	bottomRight = _corner[3];
}

