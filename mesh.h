#pragma once

#include "VAO.h"

class SEMesh
{
public:
	SEMesh();
	~SEMesh();
private:
	std::vector<VertexData> vertex;
};