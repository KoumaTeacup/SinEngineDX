/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"

void DisplaySkeleton(FbxNode* pNode)
{ 
    FbxSkeleton* lSkeleton = (FbxSkeleton*) pNode->GetNodeAttribute();

    DisplayString("Skeleton Name: ", (char *) pNode->GetName());
    DisplayMetaDataConnections(lSkeleton);

    const char* lSkeletonTypes[] = { "Root", "Limb", "Limb Node", "Effector" };

    DisplayString("    Type: ", lSkeletonTypes[lSkeleton->GetSkeletonType()]);

	FbxVector4 translation = pNode->EvaluateLocalTranslation(FBXSDK_TIME_INFINITE);
	
	FBXSDK_printf("        Translation: %f %f %f\n", translation[0], translation[1], translation[2]);

    if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimb)
    {
        DisplayDouble("    Limb Length: ", lSkeleton->LimbLength.Get());
    }
    else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
    {
        DisplayDouble("    Limb Node Size: ", lSkeleton->Size.Get());
    }
    else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eRoot)
    {
        DisplayDouble("    Limb Root Size: ", lSkeleton->Size.Get());
    }

    DisplayColor("    Color: ", lSkeleton->GetLimbNodeColor());
}
