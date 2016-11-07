#include "FBX.h"
#include "bone.h"
#include "animation.h"
#include "skeleton.h"

using namespace std;

SEFBX::SEFBX():manager(FbxManager::Create()) {
	// Create a fbx manager
	FbxIOSettings *settings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(settings);
}

SEFBX::~SEFBX() {
	if (manager) manager->Destroy();
}

void SEFBX::importScene(const char *filename, unordered_map<string, SEAsset*> &allAssets) {

	FbxScene* scene = FbxScene::Create(manager, filename);

	auto timeMode = scene->GetGlobalSettings().GetTimeMode();

	// Create a importer
	FbxImporter *importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename, -1, manager->GetIOSettings());

	// Import the new scene
	if (!scene) return;

	importer->Import(scene);
	importer->Destroy();

	FbxNode *root = scene->GetRootNode();
	int numChildren = root->GetChildCount();

	// Loop through the scene
	for (int iNode = 0; iNode < numChildren; iNode++) {
		FbxNode *child = root->GetChild(iNode);
		FbxNodeAttribute::EType attributeType = child->GetNodeAttribute()->GetAttributeType();

		SESkeleton *newSkeleton;
		SEMesh *newMesh;
		string assetName;

		switch (attributeType) {
		case FbxNodeAttribute::eSkeleton:
			// Import bone nodes
			newSkeleton = new SESkeleton();
			newSkeleton->setTimeMode(timeMode);
			assetName = string(filename);
			assetName = assetName.substr(0, assetName.size() - 4) + ":" + string(child->GetName());
			newSkeleton->setAssetName(assetName);
			newSkeleton->loadSkeleton(importSkeleton(child, scene));
			allAssets[newSkeleton->getAssetName()] = newSkeleton;
			break;

		case FbxNodeAttribute::eMesh:
			newMesh = importMesh(child, scene);
			assetName = string(filename);
			assetName = assetName.substr(0, assetName.size() - 4) + ":" + string(child->GetName());
			newMesh->setAssetName(assetName);
			allAssets[newMesh->getAssetName()] = newMesh;
			break;
		default:
			break;
		}
	}

	scene->Destroy();
}

SEBone * SEFBX::importSkeleton(FbxNode * skeletonNode, FbxScene * scene)
{
	SEBone *newBone = new SEBone();

	auto timeMode = scene->GetGlobalSettings().GetTimeMode();

	// local transform
	FbxVector4 fbxTrans = skeletonNode->LclTranslation.Get();
	FbxVector4 fbxRot = skeletonNode->LclRotation.Get();
	fbxRot[0] = fbxRot[0] / 180.0f * XM_PI;
	fbxRot[1] = fbxRot[1] / 180.0f * XM_PI;
	fbxRot[2] = fbxRot[2] / 180.0f * XM_PI;
	FbxVector4 fbxScl = skeletonNode->LclScaling.Get();

	DirectX::XMFLOAT3 localTranslate((float)fbxTrans[0], (float)fbxTrans[1], (float)fbxTrans[2]);
	DirectX::XMFLOAT3 localRotate((float)fbxRot[0], (float)fbxRot[1], (float)fbxRot[2]);
	DirectX::XMFLOAT3 localScale((float)fbxScl[0], (float)fbxScl[1], (float)fbxScl[2]);

	// joint orient
	FbxVector4 fbxRotate = skeletonNode->GetPreRotation(FbxNode::eSourcePivot);
	fbxRotate[0] = fbxRotate[0] / 180.0f * XM_PI;
	fbxRotate[1] = fbxRotate[1] / 180.0f * XM_PI;
	fbxRotate[2] = fbxRotate[2] / 180.0f * XM_PI;
	XMVECTOR orientVec = XMVectorSet((float)fbxRotate[0], (float)fbxRotate[1], (float)fbxRotate[2], 0.0f);
	SEQuaternion boneOrient = SEQuaternion::fromEuler(XMVectorSet((float)fbxRotate[0], (float)fbxRotate[1], (float)fbxRotate[2], 0.0f));

	newBone->loadBone(skeletonNode->GetName(), localTranslate, localRotate, localScale, boneOrient);

	// animation curve
	int numTakes = scene->GetSrcObjectCount<FbxAnimStack>();
	for (int iTake = 0; iTake < numTakes; iTake++) {
		SEAnimation *animation = new SEAnimation(newBone);
		FbxAnimCurve *curve;
		// Assuming 1 layer
		FbxAnimLayer *animlayer = scene->GetSrcObject<FbxAnimStack>(iTake)->GetMember<FbxAnimLayer>(0);
		curve = skeletonNode->LclTranslation.GetCurve(animlayer, FBXSDK_CURVENODE_COMPONENT_X);
		animation->insert(curve, SE_ELEMENT_TX, timeMode);
		curve = skeletonNode->LclTranslation.GetCurve(animlayer, FBXSDK_CURVENODE_COMPONENT_Y);
		animation->insert(curve, SE_ELEMENT_TY, timeMode);
		curve = skeletonNode->LclTranslation.GetCurve(animlayer, FBXSDK_CURVENODE_COMPONENT_Z);
		animation->insert(curve, SE_ELEMENT_TZ, timeMode);
		curve = skeletonNode->LclRotation.GetCurve(animlayer, FBXSDK_CURVENODE_COMPONENT_X);
		animation->insert(curve, SE_ELEMENT_RX, timeMode);
		curve = skeletonNode->LclRotation.GetCurve(animlayer, FBXSDK_CURVENODE_COMPONENT_Y);
		animation->insert(curve, SE_ELEMENT_RY, timeMode);
		curve = skeletonNode->LclRotation.GetCurve(animlayer, FBXSDK_CURVENODE_COMPONENT_Z);
		animation->insert(curve, SE_ELEMENT_RZ, timeMode);
		curve = skeletonNode->LclScaling.GetCurve(animlayer, FBXSDK_CURVENODE_COMPONENT_X);
		animation->insert(curve, SE_ELEMENT_SX, timeMode);
		curve = skeletonNode->LclScaling.GetCurve(animlayer, FBXSDK_CURVENODE_COMPONENT_Y);
		animation->insert(curve, SE_ELEMENT_SY, timeMode);
		curve = skeletonNode->LclScaling.GetCurve(animlayer, FBXSDK_CURVENODE_COMPONENT_Z);
		animation->insert(curve, SE_ELEMENT_SZ, timeMode);

		animation->Init();

		newBone->addAnimation(animation);
	}

	newBone->Reset();

	for (int i = 0; i < skeletonNode->GetChildCount(); i++) {
		newBone->addChild(importSkeleton(skeletonNode->GetChild(i), scene));
	}

	return newBone;
}

SEMesh * SEFBX::importMesh(FbxNode * meshNode, FbxScene * scene)
{
	SEMesh *newMesh = new SEMesh();
	newMesh->setAssetName(meshNode->GetName());
	FbxMesh *pMesh = (FbxMesh*)meshNode->GetNodeAttribute();

	int CPCount = pMesh->GetControlPointsCount();
	FbxVector4* CP = pMesh->GetControlPoints();
	vector<ControlPoint> CPList;

	// Copy Control points to CPList
	for (int i = 0; i < CPCount; i++) {
		CPList.push_back(ControlPoint(CP[i]));
		CPList.back().index = i;
	}

	int polygonCount = pMesh->GetPolygonCount();

	// Load Vertex data into VertList
	int vertexId = 0;
	vector<int> indexList;
	// Loop through all faces
	for (int i = 0; i < polygonCount; i++) {

		// contruct the index array
		int polygonSize = pMesh->GetPolygonSize(i);
		for (int j = 0; j < polygonSize; j++) {
			indexList.push_back(pMesh->GetPolygonVertex(i, j));

			// Assuming only 1 normal set
			FbxGeometryElementNormal *normalList = pMesh->GetElementNormal();
			
			XMFLOAT3 newNormal;
			// This basically assumes the fbx is not using smoothing groups
			if (normalList->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				FbxVector4 normalVec4;
				switch (normalList->GetReferenceMode()) {
				case FbxGeometryElement::eDirect:
					normalVec4 = normalList->GetDirectArray().GetAt(vertexId);
					newNormal.x = (float)normalVec4[0];
					newNormal.y = (float)normalVec4[1];
					newNormal.z = (float)normalVec4[2];
					break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int id = normalList->GetIndexArray().GetAt(vertexId);
					normalVec4 = normalList->GetDirectArray().GetAt(id);
					newNormal.x = (float)normalVec4[0];
					newNormal.y = (float)normalVec4[1];
					newNormal.z = (float)normalVec4[2];
				}
					break;
				default:
					break;
				}
			}
			
			ControlPoint *pCPWithNormal = &CPList[indexList.back()];
			while (pCPWithNormal) {
				if (pCPWithNormal->updated) {
					// If this control point is indexed before
					if (pCPWithNormal->normal.x == newNormal.x &&
						pCPWithNormal->normal.y == newNormal.y &&
						pCPWithNormal->normal.z == newNormal.z)
					{
						// ... And the normal matches, update index to match the control point
						indexList.back() = pCPWithNormal->index;
						break;
					}
				}
				else {
					// If this is the first time update this control point, update it
					pCPWithNormal->normal = newNormal;
					pCPWithNormal->updated = true;
					break;
				}
				pCPWithNormal = pCPWithNormal->next;
			}

			if (!pCPWithNormal) {
				// No existing control point found matching the normal, push a new one into list
				CPList.push_back(CPList[indexList.back()]);
				CPList.back().normal = newNormal;
				CPList.back().index = CPList.size() - 1;
				indexList.back() = CPList.size() - 1;
			}

			vertexId++;
		}//polygonSize
	}//polygonCount

	// Index array is correct, now we only have to change the format to GPU-friendly VertexData struct
	vector<VertexData> vertData;
	for (auto i : CPList) {
		vertData.push_back(VertexData(i.pos, i.normal, XMFLOAT2(), XMFLOAT3(), i.normal, true));
	}

	newMesh->Load(vertData.size(), &vertData[0], indexList.size(), &indexList[0]);
	return newMesh;
}

ControlPoint::ControlPoint(const FbxVector4 & input) : updated(false), normal(XMFLOAT3()), next(nullptr)
{
	pos.x = (float)input[0];
	pos.y = (float)input[1];
	pos.z = (float)input[2];
}
