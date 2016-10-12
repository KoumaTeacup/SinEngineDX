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

	// Import bone nodes
	FbxNode *root = scene->GetRootNode();
	
	for (int i = 0; i < root->GetChildCount(); i++) {
		FbxNode *child = root->GetChild(i);
		FbxNodeAttribute::EType attributeType = child->GetNodeAttribute()->GetAttributeType();
		switch (attributeType) {
		case FbxNodeAttribute::eSkeleton:
			loadSkeleton(child, nullptr); 
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

void SEScene::loadSkeleton(FbxNode * skeletonNode, SEBone *parent) {
	SEBone *newBone = new SEBone();

	if (parent) 
		parent->addChild(newBone);
	else 
		skeletons.push_back(newBone);

	FbxVector4 fbxTrans = skeletonNode->EvaluateLocalTransform();
	FbxVector4 fbxRotate = skeletonNode->GetPreRotation(FbxNode::eSourcePivot);
	DirectX::XMFLOAT3 localTranslate((float)fbxTrans[0], (float)fbxTrans[1], (float)fbxTrans[2]);
	DirectX::XMFLOAT3 boneOrient((float)fbxRotate[0], (float)fbxRotate[0], (float)fbxRotate[0]);

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
