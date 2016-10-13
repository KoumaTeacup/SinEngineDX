#include "scene.h"
#include "bone.h"
#include "d3dUtil.h"

SEScene::SEScene(FbxManager * manager, const char * filename) :
	sceneName(filename) {

	FbxScene* scene = FbxScene::Create(manager, filename);

	// Create a importer
	FbxImporter *importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename, -1, manager->GetIOSettings());

	// Import the new scene
	importer->Import(scene);
	importer->Destroy();

	FbxNode *root = scene->GetRootNode();
	int numChildren = root->GetChildCount();

	// Loop through the scene
	for (int iNode = 0; iNode < numChildren; iNode++) {
		FbxNode *child = root->GetChild(iNode);
		FbxNodeAttribute::EType attributeType = child->GetNodeAttribute()->GetAttributeType();

		switch (attributeType) {
		case FbxNodeAttribute::eSkeleton:
			// Import bone nodes
			loadSkeleton(child, nullptr, scene);
			break;
		default:
			break;
		}
	}

	scene->Destroy();
}

SEScene::~SEScene() {
	for (auto i : skeletons) {
		delete i;
	}
}

void SEScene::loadSkeleton(FbxNode * skeletonNode, SEBone *parent, FbxScene *scene) {
	SEBone *newBone = new SEBone();

	if (parent) 
		parent->addChild(newBone);
	else 
		skeletons.push_back(newBone);

	// local transform
	FbxVector4 fbxTrans = skeletonNode->EvaluateLocalTransform();
	FbxVector4 fbxRotate = skeletonNode->GetPreRotation(FbxNode::eSourcePivot);
	DirectX::XMFLOAT3 localTranslate((float)fbxTrans[0], (float)fbxTrans[1], (float)fbxTrans[2]);
	DirectX::XMFLOAT3 boneOrient((float)fbxRotate[0], (float)fbxRotate[0], (float)fbxRotate[0]);

	// animation curve

	int numTakes = scene->GetSrcObjectCount<FbxAnimStack>();
	for (int iTake = 0; iTake < numTakes; iTake++) {
		// Assuming 1 layer
		FbxAnimLayer *animlayer = scene->GetSrcObject<FbxAnimStack>(iTake)->GetMember<FbxAnimLayer>(0);
		skeletonNode->LclTranslation.GetCurve(animlayer, FBXSDK_CURVENODE_COMPONENT_X);
	}

	newBone->loadBone(skeletonNode->GetName(), localTranslate, boneOrient);

	for (int i = 0; i < skeletonNode->GetChildCount(); i++) {
		loadSkeleton(skeletonNode->GetChild(i), newBone);
	}

}

void SEScene::drawSkeleton() {
	for (auto i : skeletons) {
		i->drawAll();
	}
}
