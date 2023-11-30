#include "Globals.h"
#include "MyModel.h"
#include "Application.h"
#include "ModuleTexture.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION

#include "./include/tinygltf/tiny_gltf.h"

using namespace tinygltf;
MyModel::MyModel() {

}
MyModel::~MyModel() {

}

void MyModel::Load(const char* assetFileName)
{
	const char* filePath = App->CreateFilePath(MODEL_PATH, assetFileName);

	TinyGLTF gltfContext;
	Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, filePath);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
	}

	for (const auto& srcMesh : model.meshes)
	{

		for (const auto& primitive : srcMesh.primitives)
		{
			MyMesh* mesh = new MyMesh();
			mesh->LoadMesh(model, srcMesh, primitive);			
			
			Meshes.push_back(mesh);
		}
	}
	LoadMaterials(model);
}

void MyModel::LoadMaterials(const tinygltf::Model& srcModel)
{
	for (const auto& srcMaterial : srcModel.materials)
	{  
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const Image& image = srcModel.images[texture.source];
			
			const char* imageUri = image.uri.c_str();
			textureId = (App->GeTexture()->LoadTexture(imageUri));
		}
		Textures.push_back(textureId);
	}
}

void MyModel::Draw()
{
	for (int i = 0; i < Meshes.size(); i++) {
		Meshes[i]->RenderSeparatedArrays();
		Meshes[i]->Draw(Textures);
	}
}
