#include "asset.h"

class SEBone;
class SEAnimation;

class SESkeleton : public SEAsset
{
public:
	SESkeleton();
	~SESkeleton();

	void loadSkeleton(SEBone *_boneRoot) { boneRoot = _boneRoot; }

	void virtual Draw();
	void virtual Tick();
	void virtual Pause();
	void virtual Resume();

private:

	SEBone *boneRoot;
};