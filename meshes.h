#pragma once

#include "VAO.h"

namespace SE_Internal_Mesh
{

const float sin30 = sinf(3.14159265f / 6.0f);
const float cos30 = cosf(3.14159265f / 6.0f);

// Triangle
const int triangleVCount = 3;
const VertexData triangleV[3] = {
	VertexData(XMFLOAT3(-1.0f, -1.0f, 0.0f)),
	VertexData(XMFLOAT3(0.0f, 1.0f, 1.0f)),
	VertexData(XMFLOAT3(1.0f, -1.0f, 0.0f))
};
const int triangleICount = 3;
const int triangleI[3] = {0, 1, 2};

// Box
const int boxVCount = 8;
const VertexData boxV[8] = {
	VertexData(XMFLOAT3(-1.0f, -1.0f, -1.0f)),	// 0
	VertexData(XMFLOAT3(-1.0f, 1.0f, -1.0f)),	// 1
	VertexData(XMFLOAT3(1.0f, -1.0f, -1.0f)),	// 2
	VertexData(XMFLOAT3(1.0f, 1.0f, -1.0f)),	// 3
	VertexData(XMFLOAT3(-1.0f, -1.0f, 1.0f)),	// 4
	VertexData(XMFLOAT3(-1.0f, 1.0f, 1.0f)),	// 5
	VertexData(XMFLOAT3(1.0f, -1.0f, 1.0f)),	// 6
	VertexData(XMFLOAT3(1.0f, 1.0f, 1.0f))		// 7
};
const int boxICount = 36;
const int boxI[36] = {
	// front
	0,1,3,0,3,2,
	// back
	5,4,6,5,6,7,
	// left
	4,5,1,4,1,0,
	// right
	2,3,7,2,7,6,
	// top
	1,5,7,1,7,3,
	// bottom
	4,0,2,4,2,6
};

// Bone Mesh
const int boneMeshVCount = 5;
const VertexData boneMeshV[5] = {
	VertexData(XMFLOAT3(0.0f, 0.7f, -0.7f), XMFLOAT3(1.0f, 1.0f, 1.0f)),	// 0
	VertexData(XMFLOAT3(0.0f, -0.7f, -0.7f), XMFLOAT3(1.0f, 1.0f, 1.0f)),	// 1
	VertexData(XMFLOAT3(0.0f, 0.7f, 0.7f), XMFLOAT3(1.0f, 1.0f, 1.0f)),	// 2
	VertexData(XMFLOAT3(0.0f, -0.7f, 0.7f), XMFLOAT3(1.0f, 1.0f, 1.0f)),	// 3
	VertexData(XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)),		// 4
};
const int boneMeshICount = 12;
const int boneMeshI[12] = {
	// sides
	0,4,1,1,4,3,3,4,2,2,4,0,
};

// Joint Mesh
const int jointMeshVCount = 31;
const VertexData jointMeshV[31] = {
	VertexData(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)),			// 0
	VertexData(XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)),		// 1
	VertexData(XMFLOAT3(0.0f, sin30, -cos30), XMFLOAT3(1.0f, 0.0f, 0.0f)),		// 2
	VertexData(XMFLOAT3(0.0f, cos30, -sin30), XMFLOAT3(1.0f, 0.0f, 0.0f)),		// 3
	VertexData(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)),			// 4
	VertexData(XMFLOAT3(0.0f, cos30, sin30), XMFLOAT3(1.0f, 0.0f, 0.0f)),		// 5
	VertexData(XMFLOAT3(0.0f, sin30, cos30), XMFLOAT3(1.0f, 0.0f, 0.0f)),		// 6
	VertexData(XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)),			// 7
	VertexData(XMFLOAT3(0.0f, -sin30, cos30), XMFLOAT3(1.0f, 0.0f, 0.0f)),		// 8
	VertexData(XMFLOAT3(0.0f, -cos30, sin30), XMFLOAT3(1.0f, 0.0f, 0.0f)),		// 9
	VertexData(XMFLOAT3(0.0f, -1.0, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)),			// 10
	VertexData(XMFLOAT3(0.0f, -cos30, -sin30), XMFLOAT3(1.0f, 0.0f, 0.0f)),		// 11
	VertexData(XMFLOAT3(0.0f, -sin30, -cos30), XMFLOAT3(1.0f, 0.0f, 0.0f)),		// 12
	VertexData(XMFLOAT3(-sin30, 0.0f, -cos30), XMFLOAT3(0.0f, 1.0f, 0.0f)),		// 13
	VertexData(XMFLOAT3(-cos30, 0.0f, -sin30), XMFLOAT3(0.0f, 1.0f, 0.0f)),		// 14
	VertexData(XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)),		// 15
	VertexData(XMFLOAT3(-cos30, 0.0f, sin30), XMFLOAT3(0.0f, 1.0f, 0.0f)),		// 16
	VertexData(XMFLOAT3(-sin30, 0.0f, cos30), XMFLOAT3(0.0f, 1.0f, 0.0f)),		// 17
	VertexData(XMFLOAT3(sin30, 0.0f, cos30), XMFLOAT3(0.0f, 1.0f, 0.0f)),		// 18
	VertexData(XMFLOAT3(cos30, 0.0f, sin30), XMFLOAT3(0.0f, 1.0f, 0.0f)),		// 19
	VertexData(XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)),			// 20
	VertexData(XMFLOAT3(cos30, 0.0f, -sin30), XMFLOAT3(0.0f, 1.0f, 0.0f)),		// 21
	VertexData(XMFLOAT3(sin30, 0.0f, -cos30), XMFLOAT3(0.0f, 1.0f, 0.0f)),		// 22
	VertexData(XMFLOAT3(-cos30, sin30, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),		// 23
	VertexData(XMFLOAT3(-sin30, cos30, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),		// 24
	VertexData(XMFLOAT3(sin30, cos30, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),		// 25
	VertexData(XMFLOAT3(cos30, sin30, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),		// 26
	VertexData(XMFLOAT3(cos30, -sin30, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),		// 27
	VertexData(XMFLOAT3(sin30, -cos30, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),		// 28
	VertexData(XMFLOAT3(-sin30, -cos30, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),		// 29
	VertexData(XMFLOAT3(-cos30, -sin30, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),		// 30
};
const int jointMeshICount = 40;
const int jointMeshI[40] = {
	// x-plane
	1,2,3,4,5,6,7,8,9,10,11,12,
	// y-plane
	1,13,14,15,16,17,7,18,19,20,21,22,
	// z-plane
	1,13,14,15,23,24,4,25,26,20,27,28,10,29,30,15
};

}