#pragma once

#include "stdafx.h"

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
		vector<FbxAMatrix>	offsetMatrices;
	}MESHINFO;

private:
	FbxManager* _pFbxManager;

	unique_ptr<vector<FbxNode*>> _bones;
	unique_ptr<string> _name;

public:
	FBXParser();
	~FBXParser();

	void ParseAndExport(const string& fileName);

private:
	void parsePrefab(FbxNode* pFbxNode);

	void parseTransformRecursively(FbxNode* pFbxNode, Json::Value& json, int index, int parentIndex);
	void parseRendererRecursively(FbxNode* pFbxNode, Json::Value& json, int index);

	void parseRenderer(FbxNode* pFbxNode, Json::Value& json);
	void parseTransform(FbxNode* pFbxNode, Json::Value& json);

	void parseMesh(FbxMesh* pMesh, Json::Value& json);
	void parseMaterial(FbxSurfaceMaterial* pFbxMaterial, const string& materialName);
	void parseAnimation(FbxScene* pScene);

	void readPosition(FbxMesh* pFbxMesh, int ctrlPointIndex, VERTEXINFO& json);
	void readBlendingInfo(FbxMesh* pMesh, MESHINFO& cotrlPoints);
	void readUV(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json);
	void readNormal(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json);
	void readTangent(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json);
	void readBinormal(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json);
};

