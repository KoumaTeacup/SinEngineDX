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
	void virtual Tick(float dt);
	void virtual Pause();
	void virtual Resume();

	SEBone* getRootBone() { return boneRoot; }

private:

	SEBone *boneRoot;
};