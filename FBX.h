#pragma once

#include <fbxsdk.h>
#include <vector>

#include "scene.h"

class SEFBX
{
public:
	SEFBX();
	~SEFBX();

	SEScene *importScene(const char *filename);
private:
	FbxManager *manager;
};