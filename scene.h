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
	void Draw();

	void Tick();
	void Pause();
	void Resume();

private:

	FbxTime::EMode timeMode;
	std::string sceneName;
	std::vector<SEBone *> skeletons;
};