#pragma once
#include "Application.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "./include/tinygltf/tiny_gltf.h"

#include "MyMesh.h"
#include <vector>
using namespace tinygltf;
class MyModel
{
public:
	MyModel();
	MyModel(const char* assetFileName, bool exterior = false);
	~MyModel();
	void Load(const char* assetFileName, bool exterior = false);
	void LoadMaterials(const tinygltf::Model& srcModel);
	void Draw();

private:
	std::vector<MyMesh*> Meshes;
	std::vector<unsigned int> Textures;
};

