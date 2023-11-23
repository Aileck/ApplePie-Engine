#pragma once
#include <vector>

#include "MyMesh.h"


class MyModel
{
public:
	MyModel();
	~MyModel();
	void Load(const char* assetFileName);

private:
	std::vector<MyMesh*> m_Meshes;
};

