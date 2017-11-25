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
	_name = make_unique<string>();

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

	*_name = split(split(fileName, '\\')->back(), '.')->front();

	parsePrefab(pRootNode);
	parseAnimation(pScene);
}

void FBXParser::parsePrefab(FbxNode* pFbxNode)
{
	Json::Value prefab;

	parseTransformRecursively(pFbxNode, prefab, 0, -1);
	parseRendererRecursively(pFbxNode, prefab, 0);

	if (prefab["GameObject"].size() > 0) prefab["GameObject"][0]["name"] = *_name;

	JsonUtility::WriteToFile("Assets\\prefab\\" + *_name + ".prefab", prefab);
}

void FBXParser::parseTransformRecursively(FbxNode* pFbxNode, Json::Value& json, int index, int parentIndex)
{
	Json::Value newGameObject;
	newGameObject["name"] = pFbxNode->GetName();
	newGameObject["components"]["Transform"] = json["Transform"].size();
	newGameObject["parent_index"] = parentIndex;
	parseTransform(pFbxNode, json);

	json["GameObject"].append(newGameObject);

	for (int i = 0; i < pFbxNode->GetChildCount(); ++i)
		parseTransformRecursively(pFbxNode->GetChild(i), json, json["GameObject"].size(), index);
}

void FBXParser::parseRendererRecursively(FbxNode* pFbxNode, Json::Value& json, int index)
{
	auto pMesh = pFbxNode->GetMesh();

	if (pMesh)
	{
		auto key = pMesh->GetDeformerCount() > 0 ? "SkinnedMeshRenderer" : "MeshRenderer";
		json["GameObject"][index]["components"][key] = pMesh->GetDeformerCount() > 0 ? json["SkinnedMeshRenderer"].size() : json["MeshRenderer"].size();
		parseRenderer(pFbxNode, json);
	}

	for (int i = 0; i < pFbxNode->GetChildCount(); ++i)
		parseRendererRecursively(pFbxNode->GetChild(i), json, ++index);

}

void FBXParser::parseRenderer(FbxNode* pFbxNode, Json::Value& prefab)
{
	Json::Value newRenderer;

	newRenderer["mesh"] = "Assets\\mesh\\" + string(pFbxNode->GetName()) + ".mesh";

	Json::Value newMesh;
	for (int i = 0; i < pFbxNode->GetNodeAttributeCount(); ++i)
	{
		auto pMesh = FbxCast<FbxMesh>(pFbxNode->GetNodeAttributeByIndex(i));
		auto pMaterial = pFbxNode->GetMaterial(pFbxNode->GetNodeAttributeIndex(pMesh));
		auto materialName = "Assets\\material\\" + *_name + "_" + pMaterial->GetName() + ".material";

		parseMesh(pMesh, newMesh);
		parseMaterial(pMaterial, materialName);

		newRenderer["materials"].append("Assets\\material\\" + *_name + "_" + pMaterial->GetName() + ".material");
	}

	JsonUtility::WriteToFile(newRenderer["mesh"].asCString(), newMesh);

	if (pFbxNode->GetMesh()->GetDeformerCount() == 0)
		prefab["MeshRenderer"].append(newRenderer);
	else {

		Json::Value bones;
		for (auto bone : *_bones)
			bones.append(bone->GetName());

		newRenderer["bones"] = bones;
		if (_bones->size() > 0) newRenderer["root_bone"] = _bones->data()[0]->GetName();
		prefab["SkinnedMeshRenderer"].append(newRenderer);
	}
}

void FBXParser::parseTransform(FbxNode* pFbxNode, Json::Value& prefab)
{
	Json::Value newTransform;
	auto localMatrix = pFbxNode->GetParent() ?
		pFbxNode->GetParent()->EvaluateGlobalTransform().Inverse() * pFbxNode->EvaluateGlobalTransform()  :
		pFbxNode->EvaluateGlobalTransform();

	newTransform["scale"]["x"] = localMatrix.GetS().mData[0];
	newTransform["scale"]["y"] = localMatrix.GetS().mData[1];
	newTransform["scale"]["z"] = localMatrix.GetS().mData[2];
	newTransform["rotation"]["x"] = localMatrix.GetQ().mData[0];
	newTransform["rotation"]["y"] = localMatrix.GetQ().mData[1];
	newTransform["rotation"]["z"] = localMatrix.GetQ().mData[2];
	newTransform["rotation"]["w"] = localMatrix.GetQ().mData[3];
	newTransform["translation"]["x"] = localMatrix.GetT().mData[0];
	newTransform["translation"]["y"] = localMatrix.GetT().mData[1];
	newTransform["translation"]["z"] = localMatrix.GetT().mData[2];

	prefab["Transform"].append(newTransform);

	if (pFbxNode->GetNodeAttribute() && pFbxNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton) _bones->push_back(pFbxNode);
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

		for (auto bone : *_bones)
		{
			Json::Value keyFrame;

			keyFrame["bone_name"] = bone->GetName();

			for (FbxLongLong l = start.GetFrameCount(FbxTime::eFrames24); l <= end.GetFrameCount(FbxTime::eFrames24); ++l)
			{
				Json::Value key;

				FbxTime currTime;
				currTime.SetFrame(l, FbxTime::eFrames24);

				auto localMatrix = bone->GetParent() ?
					bone->GetParent()->EvaluateGlobalTransform(currTime).Inverse() * bone->EvaluateGlobalTransform(currTime) :
					bone->EvaluateGlobalTransform(currTime);

				key["frame"] = static_cast<int>(l);
				key["scale"]["x"] = static_cast<float>(localMatrix.GetS().mData[0]);
				key["scale"]["y"] = static_cast<float>(localMatrix.GetS().mData[1]);
				key["scale"]["z"] = static_cast<float>(localMatrix.GetS().mData[2]);
				key["rotation"]["x"] = static_cast<float>(localMatrix.GetQ().mData[0]);
				key["rotation"]["y"] = static_cast<float>(localMatrix.GetQ().mData[1]);
				key["rotation"]["z"] = static_cast<float>(localMatrix.GetQ().mData[2]);
				key["rotation"]["w"] = static_cast<float>(localMatrix.GetQ().mData[3]);
				key["translation"]["x"] = static_cast<float>(localMatrix.GetT().mData[0]);
				key["translation"]["y"] = static_cast<float>(localMatrix.GetT().mData[1]);
				key["translation"]["z"] = static_cast<float>(localMatrix.GetT().mData[2]);

				keyFrame["keys"].append(key);
			}

			animation["key_frames"].append(keyFrame);
		}

		JsonUtility::WriteToFile("Assets\\animation\\" + *_name + ".animation", animation);
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

	for (int i = 0; i < ctrlPointCount; ++i)
		readPosition(pMesh, i, ctrlPoints.vertices[i]);

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

			
			ctrlPoints.vertices[ctrlPointIndex].SortBlendingInfo();

			for (int k = 0; k < ctrlPoints.vertices[ctrlPointIndex].blendingInfos.size(); ++k)
			{
				Json::Value blendingInfo;

				blendingInfo["bone_index"] = ctrlPoints.vertices[ctrlPointIndex].blendingInfos[k].boneIndex;
				blendingInfo["blending_weight"] = ctrlPoints.vertices[ctrlPointIndex].blendingInfos[k].blendingWeight;

				vertex["blending_infos"].append(blendingInfo);
			}
			

			vertices.append(vertex);
			indices.append(vertexCount++);

		}
	}

	for (int i = 0; i < ctrlPoints.offsetMatrices.size(); ++i)
	{
		Json::Value matrix;

		matrix[0][0] = ctrlPoints.offsetMatrices[i].Get(0, 0);
		matrix[0][1] = ctrlPoints.offsetMatrices[i].Get(0, 1);
		matrix[0][2] = ctrlPoints.offsetMatrices[i].Get(0, 2);
		matrix[0][3] = ctrlPoints.offsetMatrices[i].Get(0, 3);
		matrix[1][0] = ctrlPoints.offsetMatrices[i].Get(1, 0);
		matrix[1][1] = ctrlPoints.offsetMatrices[i].Get(1, 1);
		matrix[1][2] = ctrlPoints.offsetMatrices[i].Get(1, 2);
		matrix[1][3] = ctrlPoints.offsetMatrices[i].Get(1, 3);
		matrix[2][0] = ctrlPoints.offsetMatrices[i].Get(2, 0);
		matrix[2][1] = ctrlPoints.offsetMatrices[i].Get(2, 1);
		matrix[2][2] = ctrlPoints.offsetMatrices[i].Get(2, 2);
		matrix[2][3] = ctrlPoints.offsetMatrices[i].Get(2, 3);
		matrix[3][0] = ctrlPoints.offsetMatrices[i].Get(3, 0);
		matrix[3][1] = ctrlPoints.offsetMatrices[i].Get(3, 1);
		matrix[3][2] = ctrlPoints.offsetMatrices[i].Get(3, 2);
		matrix[3][3] = ctrlPoints.offsetMatrices[i].Get(3, 3);

		offsetMatrices.append(matrix);
	}

	mesh["is_skinned"] = pMesh->GetDeformerCount() != 0;
	mesh["vertices"] = vertices;
	mesh["indices"] = indices;
	mesh["offset_matrices"] = offsetMatrices;

	json.append(mesh);
}

void FBXParser::parseMaterial(FbxSurfaceMaterial * pFbxMaterial, const string& materialName)
{
	Json::Value material;

	material["shader"] = "Assets\\effect\\DefaultMaterial.fx";

	JsonUtility::WriteToFile(materialName, material);
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

			pCluster->GetTransformMatrix(transformMatrix);			// The transformation of the mesh at binding time
			pCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space

			FbxAMatrix globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix;

			ctrlPoints.offsetMatrices[curBoneIdx] = globalBindposeInverseMatrix;

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

	BLENDINGINFO bi;
	bi.boneIndex = 0;
	bi.blendingWeight = 0.0f;

	for (int i = 0; i < ctrlPoints.vertices.size(); ++i)
	{
		for (int j = ctrlPoints.vertices[i].blendingInfos.size(); j < 4; ++j)
			ctrlPoints.vertices[i].blendingInfos.push_back(bi);
	}
}

void FBXParser::readPosition(FbxMesh* pFbxMesh, int ctrlPointCount, VERTEXINFO& vertex)
{
	vertex.position.x = float(pFbxMesh->GetControlPointAt(ctrlPointCount).mData[0]);
	if (_bones->size() > 0)
	{
		vertex.position.y = float(pFbxMesh->GetControlPointAt(ctrlPointCount).mData[1]);
		vertex.position.z = float(pFbxMesh->GetControlPointAt(ctrlPointCount).mData[2]);
	}
	else
	{
		vertex.position.y = float(pFbxMesh->GetControlPointAt(ctrlPointCount).mData[2]);
		vertex.position.z = float(pFbxMesh->GetControlPointAt(ctrlPointCount).mData[1]);
	}
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