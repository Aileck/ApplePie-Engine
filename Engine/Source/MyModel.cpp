#include "Globals.h"
#include "MyModel.h"
#include "Application.h"
#include "ModuleTexture.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include "./include/tinygltf/tiny_gltf.h"

using namespace tinygltf;
MyModel::MyModel() {

}
MyModel::~MyModel() {

}

void MyModel::Load(const char* assetFileName)
{
	TinyGLTF gltfContext;
	Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
	}

	for (const auto& srcMesh : model.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			MyMesh* mesh = new MyMesh();
			mesh->LoadVAO(model, srcMesh, primitive);
		}
	}
}

void MyModel::LoadMaterials(const tinygltf::Model& srcModel)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const Image& image = srcModel.images[texture.source];\
			
			const char* imageUri = image.uri.c_str();
			textureId = (App->GeTexture()->LoadTexture(imageUri));
		}
		m_Textures.push_back(textureId);
	}
}
