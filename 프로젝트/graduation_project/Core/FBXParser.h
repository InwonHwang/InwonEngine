#pragma once

#include "..\pch.h"
#include "Serializable.h"
#include "Type.h"

class FBXParser
{
	typedef struct tagBlendingInfo
	{
		int boneIndex;
		float blendingWeight;

		bool operator < (const tagBlendingInfo& rhs) {
			return (blendingWeight > rhs.blendingWeight);
		}
	}BLENDINGINFO;

	typedef struct tagVertexInfo
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texCoord;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
		vector<BLENDINGINFO> blendingInfos;

		void SortBlendingInfo() {
			std::sort(blendingInfos.begin(), blendingInfos.end());
		}
	}VERTEXINFO;

	typedef struct tagMeshInfo
	{
		vector<VERTEXINFO>	vertices; //vertex Data
		vector<int>			indices;  //indeice Data
		vector<D3DXMATRIX>	offsetMatrices;
	}MESHINFO;

private:
	FbxManager* _pFbxManager;
	FbxImporter* pFbxImporter;
	FbxScene*	pScene;

	unique_ptr<vector<FbxNode*>> _bones;

public:
	FBXParser();
	~FBXParser();

	void ParseAndExport(const string& fileName);

private:
	void exportJson(const string& fileName, Json::Value& json);

	void parsePrefab(FbxNode* pFbxNode, const string& prefabName);
	void parseRenderer(FbxNode* pFbxNode, Json::Value& json);
	void parseTransform(FbxNode* pFbxNode, Json::Value& json);

	void parseMesh(FbxMesh* pMesh, Json::Value& json);
	void parseMaterial(FbxSurfaceMaterial* pFbxMaterial);
	void parseAnimation(FbxScene* pScene);

	void readPosition(FbxMesh* pFbxMesh, int ctrlPointIndex, VERTEXINFO& json);
	void readBlendingInfo(FbxMesh* pMesh, MESHINFO& cotrlPoints);
	void readUV(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json);
	void readNormal(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json);
	void readTangent(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json);
	void readBinormal(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json);

	D3DXQUATERNION ConvertFbxToD3DX(const FbxQuaternion &m);
	D3DXVECTOR4 ConvertFbxToD3DX(const FbxDouble4 &m);
	D3DXVECTOR3 ConvertFbxToD3DX(const FbxDouble3 &m);
	D3DXMATRIX ConvertFbxToD3DX(FbxAMatrix &mtx);

	shared_ptr<D3DXQUATERNION> ConvertToD3DX(const FbxQuaternion &m);
	shared_ptr<D3DXVECTOR4> ConvertToD3DX(const FbxDouble4 &m);
	shared_ptr<D3DXVECTOR3> ConvertToD3DX(const FbxDouble3 &m);
	shared_ptr<D3DXMATRIX> ConvertToD3DX(FbxAMatrix &mtx);
};

