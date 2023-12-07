#include "Globals.h"
#include "MyModel.h"
#include "Application.h"
#include "ModuleTexture.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION

#include "./include/tinygltf/tiny_gltf.h"
#include "FileComponent.h"


using namespace tinygltf;
MyModel::MyModel() {

}

MyModel::MyModel(const char* assetFileName, bool exterior) {
	Load(assetFileName,exterior);
}

MyModel::~MyModel() {

}

void MyModel::Load(const char* assetFileName, bool exterior)
{
	const char* filePath = FileComponent::CreateFilePath(MODEL_PATH, assetFileName);
	if (exterior)
	{
		filePath = assetFileName;
	}
	//App->WriteIntoLog(INFO_LOG, "Loading from %s", filePath);
	TinyGLTF gltfContext;
	Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, filePath);
	if (!loadOk)
	{
		//App->WriteIntoLog(ERROR_LOG, "Error loading %s: %s", assetFileName, error.c_str());
	}

	size_t numMesh = 0;
	for (const auto& srcMesh : model.meshes)
	{
		//App->WriteIntoLog(ERROR_LOG, "Loading mesh id %i", numMesh);
		for (const auto& primitive : srcMesh.primitives)
		{
			if (true) {
				MyMesh* mesh = new MyMesh();
				mesh->LoadMesh(model, srcMesh, primitive);

				Meshes.push_back(mesh);
				numMesh++;
			}
		}

		
	}
	LoadMaterials(model);
}

void MyModel::LoadMaterials(const tinygltf::Model& srcModel)
{
	size_t numTex = 0;
	for (const auto& srcMaterial : srcModel.materials)
	{  
		//App->WriteIntoLog(ERROR_LOG, "Loading texture id %i", numTex);
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const Image& image = srcModel.images[texture.source];
			
			const char* imageUri = image.uri.c_str();
			textureId = (App->GeTexture()->LoadTexture(imageUri));
		}
		Textures.push_back(textureId);
		numTex++;
	}
}

void MyModel::Draw()
{
	for (int i = Meshes.size() - 1; i >= 0; i--) {
		Meshes[i]->Draw(Textures);
	}
}
