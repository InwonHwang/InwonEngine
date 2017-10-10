#include "FBXParser.h"

FBXParser::FBXParser()
{
	_pFbxManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(_pFbxManager, IOSROOT);

	// 임포트값 세팅
	ios->SetBoolProp(IMP_FBX_MATERIAL, true);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	ios->SetBoolProp(IMP_FBX_LINK, true);
	ios->SetBoolProp(IMP_FBX_SHAPE, true);
	ios->SetBoolProp(IMP_FBX_GOBO, true);
	ios->SetBoolProp(IMP_FBX_ANIMATION, true);
	ios->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	// 익스포트값 세팅
	ios->SetBoolProp(EXP_FBX_MATERIAL, true);
	ios->SetBoolProp(EXP_FBX_TEXTURE, true);
	ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
	ios->SetBoolProp(EXP_FBX_SHAPE, true);
	ios->SetBoolProp(EXP_FBX_GOBO, true);
	ios->SetBoolProp(EXP_FBX_ANIMATION, true);
	ios->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	_pFbxManager->SetIOSettings(ios);
}

FBXParser::~FBXParser()
{
	if (_pFbxManager) _pFbxManager->Destroy();
}

void FBXParser::ParseAndExport(const string& fileName)
{
	auto pScene = FbxScene::Create(_pFbxManager, "");
	auto pFbxImporter = FbxImporter::Create(_pFbxManager, "");

	_bones = make_unique<vector<FbxNode*>>();
	
	bool importStatus = pFbxImporter->Initialize(fileName.c_str(), -1, _pFbxManager->GetIOSettings());

	if (!importStatus) return;
	
	// 임포트
	pFbxImporter->Import(pScene);
	pFbxImporter->Destroy();

	// triangle방식으로 변경 (ctrl point 방식 X)
	FbxGeometryConverter converter(_pFbxManager);
	converter.Triangulate(pScene, true);
	converter.SplitMeshesPerMaterial(pScene, true);

	FbxNode* pRootNode = pScene->GetRootNode();

	if (!pRootNode) return;

	string prefabName = split(split(fileName, '\\')->back(), '.')->front();

	parsePrefab(pRootNode, prefabName);
	parseAnimation(pScene);
}

void FBXParser::parsePrefab(FbxNode* pFbxNode, const string& prefabName)
{
	Json::Value prefab;
	prefab["name"] = prefabName;
	queue<FbxNode*> queue({ pFbxNode });

	while (!queue.empty())
	{
		FbxNode* cur = queue.front();
		queue.pop();

		Json::Value newGameObject;

		newGameObject["name"] = cur->GetName();

		if (cur->GetNodeAttribute() && cur->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			auto key = cur->GetMesh()->GetDeformerCount() > 0 ? "SkinnedMeshRenderer" : "MeshRenderer";
			newGameObject["Components"][key] = cur->GetMesh()->GetDeformerCount() > 0 ? prefab["SkinnedMeshRenderers"].size() : prefab["MeshRenderers"].size();
			parseRenderer(cur, prefab);
		}

		newGameObject["Components"]["Transform"] = prefab["Transforms"].size();
		parseTransform(cur, prefab);

		for (int i = 0; i < cur->GetChildCount(); ++i)
			queue.push(cur->GetChild(i));

		prefab["GameObjects"].append(newGameObject);
	}
	
	exportJson("prefab\\" + prefabName + ".prefab", prefab);
}

void FBXParser::exportJson(const string& path, Json::Value& prefab)
{
	char directoryPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, directoryPath);

	string p = string(directoryPath) + "\\Assets\\" + path;
	ofstream stream(p, ios::trunc);

	Json::StyledStreamWriter writer;
	writer.write(stream, prefab);

	stream.close();
}

void FBXParser::parseRenderer(FbxNode* pFbxNode, Json::Value& prefab)
{
	Json::Value newRenderer;

	newRenderer["Mesh"] = pFbxNode->GetName();

	Json::Value newMesh;
	for (int i = 0; i < pFbxNode->GetNodeAttributeCount(); ++i)
	{
		auto pMesh = FbxCast<FbxMesh>(pFbxNode->GetNodeAttributeByIndex(i));
		auto pMaterial = pFbxNode->GetMaterial(pFbxNode->GetNodeAttributeIndex(pMesh));
		
		parseMesh(pMesh, newMesh);
		parseMaterial(pMaterial);

		newRenderer["Materials"].append(pMaterial->GetName());
	}

	exportJson("mesh\\" + string(pFbxNode->GetName()) + ".mesh", newMesh);

	if (pFbxNode->GetMesh()->GetDeformerCount() == 0)
		prefab["MeshRenderers"].append(newRenderer);
	else
		prefab["SkinnedMeshRenderers"].append(newRenderer);
}

void FBXParser::parseTransform(FbxNode* pFbxNode, Json::Value& prefab)
{
	Json::Value newTransform;

	newTransform["scale"]["x"] = pFbxNode->EvaluateLocalTransform().GetS().mData[0];
	newTransform["scale"]["y"] = pFbxNode->EvaluateLocalTransform().GetS().mData[1];
	newTransform["scale"]["z"] = pFbxNode->EvaluateLocalTransform().GetS().mData[2];
	newTransform["rotation"]["x"] = pFbxNode->EvaluateLocalTransform().GetR().mData[0];
	newTransform["rotation"]["y"] = pFbxNode->EvaluateLocalTransform().GetR().mData[1];
	newTransform["rotation"]["z"] = pFbxNode->EvaluateLocalTransform().GetR().mData[2];
	newTransform["rotation"]["w"] = pFbxNode->EvaluateLocalTransform().GetR().mData[3];
	newTransform["translation"]["x"] = pFbxNode->EvaluateLocalTransform().GetT().mData[0];
	newTransform["translation"]["y"] = pFbxNode->EvaluateLocalTransform().GetT().mData[1];
	newTransform["translation"]["z"] = pFbxNode->EvaluateLocalTransform().GetT().mData[2];
	newTransform["parent_index"] = int(prefab["GameObjects"].size() - 1);

	prefab["Transforms"].append(newTransform);

	if (!pFbxNode->GetMesh()) _bones->push_back(pFbxNode);
}

void FBXParser::parseAnimation(FbxScene* pScene)
{
	int animCount = pScene->GetSrcObjectCount<FbxAnimStack>();

	for (int ac = 0; ac < animCount; ++ac)
	{
		Json::Value animation;

		FbxAnimStack* pAnimStack = pScene->GetSrcObject<FbxAnimStack>(ac);
		FbxTakeInfo* takeInfo = pScene->GetTakeInfo(pAnimStack->GetName());

		if (!takeInfo) continue;

		FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
		FbxTime end = takeInfo->mLocalTimeSpan.GetStop();

		animation["start_frame"] = static_cast<int>(start.GetFrameCount(FbxTime::eFrames24));
		animation["end_frame"] = static_cast<int>(end.GetFrameCount(FbxTime::eFrames24));

		for (int i = 0; i < _bones->size(); ++i)
		{
			Json::Value keyFrame;

			keyFrame["bone_name"] = _bones->data()[i]->GetName();

			for (FbxLongLong l = start.GetFrameCount(FbxTime::eFrames24); l <= end.GetFrameCount(FbxTime::eFrames24); ++l)
			{
				Json::Value key;

				FbxTime currTime;
				currTime.SetFrame(l, FbxTime::eFrames24);
				auto matrix = _bones->data()[i]->EvaluateGlobalTransform(currTime);
				key["frame"] = (int)l;
				key["scale"]["x"] = static_cast<int>(matrix.GetS().mData[0]);
				key["scale"]["y"] = static_cast<int>(matrix.GetS().mData[1]);
				key["scale"]["z"] = static_cast<int>(matrix.GetS().mData[2]);
				key["rotation"]["x"] = static_cast<int>(matrix.GetR().mData[0]);
				key["rotation"]["y"] = static_cast<int>(matrix.GetR().mData[1]);
				key["rotation"]["z"] = static_cast<int>(matrix.GetR().mData[2]);
				key["rotation"]["w"] = static_cast<int>(matrix.GetR().mData[3]);
				key["translation"]["x"] = static_cast<int>(matrix.GetT().mData[0]);
				key["translation"]["y"] = static_cast<int>(matrix.GetT().mData[1]);
				key["translation"]["z"] = static_cast<int>(matrix.GetT().mData[2]);
				key["bone_name"] = _bones->data()[i]->GetName();

				keyFrame["keys"].append(key);
			}

			animation["key_frames"].append(keyFrame);
		}

		exportJson("animation\\" + string(pAnimStack->GetName()) + ".animation", animation);
	}
}

void FBXParser::parseMesh(FbxMesh* pMesh, Json::Value& json)
{
	// Control Point 방식으로 읽기
	// 버텍스 스키닝 정보가 Control Point방시으로만 읽어들일 수 있어서 Control Point방식으로 읽어들인 후
	// Triangle 방식으로 복사 해야함
	int ctrlPointCount = pMesh->GetControlPointsCount();

	MESHINFO ctrlPoints;
	ctrlPoints.vertices.resize(ctrlPointCount);

	for (int i = 0; i < ctrlPointCount; ++i) {
		readPosition(pMesh, i, ctrlPoints.vertices[i]);
	}

	readBlendingInfo(pMesh, ctrlPoints);

	int polygonCount = pMesh->GetPolygonCount(); // 폴리곤 개수
	int vertexCount = 0;	// index값으로 들어감.

	Json::Value mesh;
	Json::Value vertices;
	Json::Value indices;
	Json::Value offsetMatrices;

	for (int i = 0; i < polygonCount; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Json::Value vertex;

			int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);

			readNormal(pMesh, ctrlPointIndex, vertexCount, vertex);
			readUV(pMesh, ctrlPointIndex, vertexCount, vertex);
			readTangent(pMesh, ctrlPointIndex, vertexCount, vertex);
			readBinormal(pMesh, ctrlPointIndex, vertexCount, vertex);

			vertex["position"]["x"] = ctrlPoints.vertices[ctrlPointIndex].position.x;
			vertex["position"]["y"] = ctrlPoints.vertices[ctrlPointIndex].position.y;
			vertex["position"]["z"] = ctrlPoints.vertices[ctrlPointIndex].position.z;

			for (int k = 0; k < 4; ++k)
			{
				Json::Value blendingInfo;

				if (k < ctrlPoints.vertices[ctrlPointIndex].blendingInfos.size())
				{					
					blendingInfo["bone_index"] = ctrlPoints.vertices[ctrlPointIndex].blendingInfos[k].boneIndex;
					blendingInfo["blending_weight"] = ctrlPoints.vertices[ctrlPointIndex].blendingInfos[k].blendingWeight;					
				}
				else
				{
					blendingInfo["bone_index"] = 0;
					blendingInfo["blending_weight"] = 0.0f;
				}

				vertex["blending_infos"].append(blendingInfo);
			}

			vertices.append(vertex);
			indices.append(vertexCount++);
		}
	}



	for (int i = 0; i < ctrlPoints.offsetMatrices.size(); ++i)
	{
		Json::Value matrix;
		matrix[0][0] = ctrlPoints.offsetMatrices[i]._11;
		matrix[0][1] = ctrlPoints.offsetMatrices[i]._12;
		matrix[0][2] = ctrlPoints.offsetMatrices[i]._13;
		matrix[0][3] = ctrlPoints.offsetMatrices[i]._14;
		matrix[1][0] = ctrlPoints.offsetMatrices[i]._21;
		matrix[1][1] = ctrlPoints.offsetMatrices[i]._22;
		matrix[1][2] = ctrlPoints.offsetMatrices[i]._23;
		matrix[1][3] = ctrlPoints.offsetMatrices[i]._24;
		matrix[2][0] = ctrlPoints.offsetMatrices[i]._31;
		matrix[2][1] = ctrlPoints.offsetMatrices[i]._32;
		matrix[2][2] = ctrlPoints.offsetMatrices[i]._33;
		matrix[2][3] = ctrlPoints.offsetMatrices[i]._34;
		matrix[3][0] = ctrlPoints.offsetMatrices[i]._41;
		matrix[3][1] = ctrlPoints.offsetMatrices[i]._42;
		matrix[3][2] = ctrlPoints.offsetMatrices[i]._43;
		matrix[3][3] = ctrlPoints.offsetMatrices[i]._44;

		offsetMatrices.append(matrix);
	}

	mesh["is_skinned"] = pMesh->GetDeformerCount() == 0 ? false : true;
	mesh["vertices"] = vertices;
	mesh["indices"] = indices;
	mesh["offset_matrices"] = offsetMatrices;

	json.append(mesh);
}

void FBXParser::parseMaterial(FbxSurfaceMaterial * pFbxMaterial)
{
	Json::Value material;

	material["shader"] = "DefaultMaterial.fx";

	exportJson("material\\" + string(pFbxMaterial->GetName()) + ".material", material);
}

void FBXParser::readBlendingInfo(FbxMesh* pMesh, MESHINFO& ctrlPoints)
{
	int deformerCount = pMesh->GetDeformerCount();

	if (deformerCount == 0) return;

	ctrlPoints.offsetMatrices.resize(_bones->size());

	for (int i = 0; i < deformerCount; ++i)
	{
		FbxSkin* pSkin = FbxCast<FbxSkin>(pMesh->GetDeformer(i, FbxDeformer::eSkin));

		if (!pSkin)	continue;

		int clusterCount = pSkin->GetClusterCount();

		for (int j = 0; j < clusterCount; ++j)
		{
			FbxCluster* pCluster = pSkin->GetCluster(j);

			string name = pCluster->GetLink()->GetName(); // 연결된 본 이름
			int curBoneIdx = 0; // 연결된 본 인덱스

								//연결된 본의 인덱스 찾기
			for (int k = 0; k < _bones->size(); ++k) {
				if (name.compare(_bones->data()[k]->GetName()) == 0) {
					curBoneIdx = k;
					break;
				}
			}

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			pCluster->GetTransformMatrix(transformMatrix);			// The transformation of the mesh at binding time
			pCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space

			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix;

			ctrlPoints.offsetMatrices[curBoneIdx] = ConvertFbxToD3DX(globalBindposeInverseMatrix);

			double *pWeights = pCluster->GetControlPointWeights();		// 해당 본에 의한 정점의 가중치
			int *pIndices = pCluster->GetControlPointIndices();			// 해당 본에 영향을 받는 정점들																

			int numBoneVertexIndices = pCluster->GetControlPointIndicesCount();

			for (int k = 0; k < numBoneVertexIndices; k++)
			{
				float tempWeight = (float)pWeights[k];	// 영향을 받는 정점의 가중치 정도
				int tempIndex = pIndices[k];			// 영향을 받는 정점의 인덱스

				BLENDINGINFO bi;
				bi.boneIndex = curBoneIdx;
				bi.blendingWeight = tempWeight;

				ctrlPoints.vertices[tempIndex].blendingInfos.push_back(bi);
			}
		} // for j
	} // for i	
}

void FBXParser::readPosition(FbxMesh* pFbxMesh, int ctrlPointCount, VERTEXINFO& vertex)
{
	vertex.position.x = float(pFbxMesh->GetControlPointAt(ctrlPointCount).mData[0]);
	vertex.position.y = float(pFbxMesh->GetControlPointAt(ctrlPointCount).mData[1]);
	vertex.position.z = float(pFbxMesh->GetControlPointAt(ctrlPointCount).mData[2]);
}

void FBXParser::readUV(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json)
{
	if (pFbxMesh->GetElementUVCount() == 0) return;

	FbxGeometryElementUV* pUV = pFbxMesh->GetElementUV(0);

	FbxLayerElement::EMappingMode mappingMode = pUV->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = pUV->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			json["texture_coordinates"]["x"] = (float)pUV->GetDirectArray().GetAt(ctrlPointIndex).mData[0];
			json["texture_coordinates"]["y"] = (float)pUV->GetDirectArray().GetAt(ctrlPointIndex).mData[1];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pUV->GetIndexArray().GetAt(ctrlPointIndex);
			json["texture_coordinates"]["x"] = (float)pUV->GetDirectArray().GetAt(i).mData[0];
			json["texture_coordinates"]["y"] = (float)pUV->GetDirectArray().GetAt(i).mData[1];
		}
		break;
		}

		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
			json["texture_coordinates"]["x"] = (float)pUV->GetDirectArray().GetAt(vertexCount).mData[0];
			json["texture_coordinates"]["y"] = (float)pUV->GetDirectArray().GetAt(vertexCount).mData[1];
			break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = pUV->GetIndexArray().GetAt(vertexCount);
			json["texture_coordinates"]["x"] = (float)pUV->GetDirectArray().GetAt(index).mData[0];
			json["texture_coordinates"]["y"] = 1.0f - (float)pUV->GetDirectArray().GetAt(index).mData[1];
		}
		break;
		}
		break;
	}
}

void FBXParser::readNormal(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json)
{
	if (pFbxMesh->GetElementNormalCount() == 0) return;

	FbxGeometryElementNormal* pNormal = pFbxMesh->GetElementNormal(0);

	FbxLayerElement::EMappingMode mappingMode = pNormal->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = pNormal->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			json["normal"]["x"] = (float)pNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[0];
			json["normal"]["y"] = (float)pNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[1];
			json["normal"]["z"] = (float)pNormal->GetDirectArray().GetAt(ctrlPointIndex).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pNormal->GetIndexArray().GetAt(ctrlPointIndex);
			json["normal"]["x"] = (float)pNormal->GetDirectArray().GetAt(i).mData[0];
			json["normal"]["y"] = (float)pNormal->GetDirectArray().GetAt(i).mData[1];
			json["normal"]["z"] = (float)pNormal->GetDirectArray().GetAt(i).mData[2];
		}
		break;
		}

		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			json["normal"]["x"] = (float)pNormal->GetDirectArray().GetAt(vertexCount).mData[0];
			json["normal"]["y"] = (float)pNormal->GetDirectArray().GetAt(vertexCount).mData[1];
			json["normal"]["z"] = (float)pNormal->GetDirectArray().GetAt(vertexCount).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pNormal->GetIndexArray().GetAt(vertexCount);

			json["normal"]["x"] = (float)pNormal->GetDirectArray().GetAt(i).mData[0];
			json["normal"]["y"] = (float)pNormal->GetDirectArray().GetAt(i).mData[1];
			json["normal"]["z"] = (float)pNormal->GetDirectArray().GetAt(i).mData[2];

		}
		break;
		}
		break;
	}
}

void FBXParser::readTangent(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json)
{
	if (pFbxMesh->GetElementTangentCount() == 0) return;

	FbxGeometryElementTangent* pTangent = pFbxMesh->GetElementTangent(0);

	FbxLayerElement::EMappingMode mappingMode = pTangent->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = pTangent->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			json["tangent"]["x"] = (float)pTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[0];
			json["tangent"]["y"] = (float)pTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[1];
			json["tangent"]["z"] = (float)pTangent->GetDirectArray().GetAt(ctrlPointIndex).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pTangent->GetIndexArray().GetAt(ctrlPointIndex);
			json["tangent"]["x"] = (float)pTangent->GetDirectArray().GetAt(i).mData[0];
			json["tangent"]["y"] = (float)pTangent->GetDirectArray().GetAt(i).mData[1];
			json["tangent"]["z"] = (float)pTangent->GetDirectArray().GetAt(i).mData[2];
		}
		break;
		}

		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			json["tangent"]["x"] = (float)pTangent->GetDirectArray().GetAt(vertexCount).mData[0];
			json["tangent"]["y"] = (float)pTangent->GetDirectArray().GetAt(vertexCount).mData[1];
			json["tangent"]["z"] = (float)pTangent->GetDirectArray().GetAt(vertexCount).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pTangent->GetIndexArray().GetAt(vertexCount);
			json["tangent"]["x"] = (float)pTangent->GetDirectArray().GetAt(i).mData[0];
			json["tangent"]["y"] = (float)pTangent->GetDirectArray().GetAt(i).mData[1];
			json["tangent"]["z"] = (float)pTangent->GetDirectArray().GetAt(i).mData[2];
		}
		break;
		}
		break;
	}
}

void FBXParser::readBinormal(FbxMesh* pFbxMesh, int ctrlPointIndex, int vertexCount, Json::Value& json)
{
	if (pFbxMesh->GetElementBinormalCount() == 0) return;

	FbxGeometryElementBinormal* pBinormal = pFbxMesh->GetElementBinormal(0);

	FbxLayerElement::EMappingMode mappingMode = pBinormal->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = pBinormal->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			json["binormal"]["x"] = (float)pBinormal->GetDirectArray().GetAt(ctrlPointIndex).mData[0];
			json["binormal"]["y"] = (float)pBinormal->GetDirectArray().GetAt(ctrlPointIndex).mData[1];
			json["binormal"]["z"] = (float)pBinormal->GetDirectArray().GetAt(ctrlPointIndex).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pBinormal->GetIndexArray().GetAt(ctrlPointIndex);
			json["binormal"]["x"] = (float)pBinormal->GetDirectArray().GetAt(i).mData[0];
			json["binormal"]["y"] = (float)pBinormal->GetDirectArray().GetAt(i).mData[1];
			json["binormal"]["z"] = (float)pBinormal->GetDirectArray().GetAt(i).mData[2];
		}
		break;
		}

		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (referenceMode)
		{
		case FbxGeometryElement::eDirect:
		{
			json["binormal"]["x"] = (float)pBinormal->GetDirectArray().GetAt(vertexCount).mData[0];
			json["binormal"]["y"] = (float)pBinormal->GetDirectArray().GetAt(vertexCount).mData[1];
			json["binormal"]["z"] = (float)pBinormal->GetDirectArray().GetAt(vertexCount).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int i = pBinormal->GetIndexArray().GetAt(vertexCount);
			json["binormal"]["x"] = (float)pBinormal->GetDirectArray().GetAt(i).mData[0];
			json["binormal"]["y"] = (float)pBinormal->GetDirectArray().GetAt(i).mData[1];
			json["binormal"]["z"] = (float)pBinormal->GetDirectArray().GetAt(i).mData[2];
		}
		break;
		}
		break;
	}
}

D3DXQUATERNION FBXParser::ConvertFbxToD3DX(const FbxQuaternion &m) {
	return D3DXQUATERNION((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

D3DXVECTOR4 FBXParser::ConvertFbxToD3DX(const FbxDouble4 &m)
{
	return D3DXVECTOR4((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

D3DXVECTOR3 FBXParser::ConvertFbxToD3DX(const FbxDouble3 &m) {
	return D3DXVECTOR3((float)m[0], (float)m[1], (float)m[2]);
}

D3DXMATRIX FBXParser::ConvertFbxToD3DX(FbxAMatrix &mtx)
{
	D3DXMATRIX ret;

	for (int i = 0; i < 4; ++i)
	{
		ret.m[i][0] = float(mtx.Get(i, 0));
		ret.m[i][1] = float(mtx.Get(i, 1));
		ret.m[i][2] = float(mtx.Get(i, 2));
		ret.m[i][3] = float(mtx.Get(i, 3));
	}

	return ret;
}

shared_ptr<D3DXQUATERNION> FBXParser::ConvertToD3DX(const FbxQuaternion &m)
{
	return make_shared<D3DXQUATERNION>((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

shared_ptr<D3DXVECTOR4> FBXParser::ConvertToD3DX(const FbxDouble4 &m)
{
	return make_shared<D3DXVECTOR4>((float)m[0], (float)m[1], (float)m[2], (float)m[3]);
}

shared_ptr<D3DXVECTOR3> FBXParser::ConvertToD3DX(const FbxDouble3 &m)
{
	return make_shared<D3DXVECTOR3>((float)m[0], (float)m[1], (float)m[2]);
}

shared_ptr<D3DXMATRIX> FBXParser::ConvertToD3DX(FbxAMatrix &mtx)
{
	return make_shared<D3DXMATRIX>(mtx.Get(0, 0), mtx.Get(0, 1), mtx.Get(0, 2), mtx.Get(0, 3),
		mtx.Get(1, 0), mtx.Get(1, 1), mtx.Get(1, 2), mtx.Get(1, 3),
		mtx.Get(2, 0), mtx.Get(2, 1), mtx.Get(2, 2), mtx.Get(2, 3),
		mtx.Get(3, 0), mtx.Get(3, 1), mtx.Get(3, 2), mtx.Get(3, 3));
}