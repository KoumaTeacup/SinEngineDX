#pragma once

#include <fbxsdk.h>
#include <string>
#include <vector>
#include "VAO.h"

class SEBone;
class SEAnimation;

class SEScene
{
public:
	SEScene(FbxManager *manager, const char * filename);
	~SEScene();

	void loadSkeleton(FbxNode *skeletonNode, SEBone * parent, FbxScene *scene);
	void drawSkeleton();

private:

	std::string sceneName;

	std::vector<SEBone *> skeletons;
	std::vector<SEAnimation *> SEAnimation;
};