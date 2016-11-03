#include "FBX.h"
#include "bone.h"
#include "animation.h"
#include "skeleton.h"

#include <DirectXMath.h>

SEFBX::SEFBX():manager(FbxManager::Create()) {
	// Create a fbx manager
	FbxIOSettings *settings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(settings);
}

SEFBX::~SEFBX() {
	if (manager) manager->Destroy();
}

std::vector<SEAsset*> SEFBX::importScene(const char *filename) {
	std::vector<SEAsset*> imported;

	FbxScene* scene = FbxScene::Create(manager, filename);

	auto timeMode = scene->GetGlobalSettings().GetTimeMode();

	// Create a importer
	FbxImporter *importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename, -1, manager->GetIOSettings());

	// Import the new scene
	if (!scene) return std::vector<SEAsset*>();

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

		switch (attributeType) {
		case FbxNodeAttribute::eSkeleton:
			// Import bone nodes
			newSkeleton = new SESkeleton();
			newSkeleton->setTimeMode(timeMode);
			newSkeleton->setAssetName(std::string(scene->GetName()) + ":" + std::string(root->GetName()));
			newSkeleton->loadSkeleton(importSkeleton(child, scene));
			imported.push_back(newSkeleton);
			break;

		case FbxNodeAttribute::eMesh:
			// Import Mesh Data
			break;
		default:
			break;
		}
	}

	scene->Destroy();

	return imported;
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
