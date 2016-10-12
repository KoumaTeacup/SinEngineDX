#include "FBX.h"

SEFBX::SEFBX():manager(FbxManager::Create()) {
	// Create a fbx manager
	FbxIOSettings *settings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(settings);
}

SEFBX::~SEFBX() {
	if (manager) manager->Destroy();
}

SEScene *SEFBX::importScene(const char *filename) {
	return new SEScene(manager, filename);
}
