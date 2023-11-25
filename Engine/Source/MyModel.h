#pragma once
#include <vector>

#include "MyMesh.h"


class MyModel
{
public:
	MyModel();
	~MyModel();
	void Load(const char* assetFileName);
	void LoadMaterials(const tinygltf::Model& srcModel);

private:
	std::vector<MyMesh*> m_Meshes;
	std::vector<unsigned int> m_Textures;
};

