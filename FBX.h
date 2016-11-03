#pragma once

#include <fbxsdk.h>
#include <vector>

#include "asset.h"

class SEFBX
{
public:
	SEFBX();
	~SEFBX();

	std::vector<SEAsset*> importScene(const char *filename);
private:
	SEBone *importSkeleton(FbxNode *skeletonNode, FbxScene *scene);
	void importMesh(FbxNode *meshNode, FbxScene *scene);

	FbxManager *manager;
};