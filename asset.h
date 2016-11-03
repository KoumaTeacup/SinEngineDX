#pragma once

#include <fbxsdk.h>
#include <string>

class SEBone;
class SEAnimation;

enum SEAssetType {
	SE_ASSET_UNDEFINED,
	SE_ASSET_SKELETON,
	SE_ASSET_MESH
};

class SEAsset
{
public:
	SEAsset();
	~SEAsset() {};

	void setTimeMode(FbxTime::EMode _timeMode) { timeMode = _timeMode; }
	void setType(SEAssetType _type) { type = _type; }
	SEAssetType getType() const { return type; }
	void setAssetName(std::string _name) { name = _name; }

	void virtual Init() {};
	void virtual Draw() {};
	void virtual Tick() {};
	void virtual Pause() {};
	void virtual Resume() {};

protected:
	FbxTime::EMode timeMode;

private:
	SEAssetType type;
	std::string name;
};