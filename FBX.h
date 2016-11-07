#pragma once

#include <fbxsdk.h>
#include <unordered_map>
#include <DirectXMath.h>

class SEBone;
class SEMesh;
class SEAsset;

// helper struct for loading time vertex data array
struct ControlPoint {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	bool updated;
	int index;
	ControlPoint *next;
	ControlPoint(const FbxVector4 &input);
};

class SEFBX
{
public:
	SEFBX();
	~SEFBX();

	void importScene(const char *filename, std::unordered_map<std::string, SEAsset*> &allAssets);
private:
	SEBone *importSkeleton(FbxNode *skeletonNode, FbxScene *scene);
	SEMesh *importMesh(FbxNode *meshNode, FbxScene *scene);

	FbxManager *manager;
};