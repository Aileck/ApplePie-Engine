#include "Globals.h"
#include "MyModel.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "MyTexture.h"

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
	App->WriteIntoLog(INFO_LOG, "Loading from %s", filePath);
	TinyGLTF gltfContext;
	Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, filePath);
	if (!loadOk)
	{
		App->WriteIntoLog(ERROR_LOG, "Error loading %s: %s", assetFileName, error.c_str());
	}

	size_t numMesh = 0;
	for (const auto& srcMesh : model.meshes)
	{
		App->WriteIntoLog(INFO_LOG, "Loading mesh no. %i", numMesh);
		for (const auto& primitive : srcMesh.primitives)
		{
			if (true) {
				MyMesh* mesh = new MyMesh();
				mesh->LoadMesh(model, srcMesh, primitive);

				Meshes.push_back(mesh);
				numMesh++;

				// Compute max/min points
				maxBoundX = (maxBoundX < mesh->GetMaxX()) ? mesh->GetMaxX() : maxBoundX;
				maxBoundY = (maxBoundY < mesh->GetMaxY()) ? mesh->GetMaxY() : maxBoundY;
				maxBoundZ = (maxBoundZ < mesh->GetMaxZ()) ? mesh->GetMaxZ() : maxBoundZ;

				minBoundX = (minBoundX > mesh->GetMinX()) ? mesh->GetMinX() : minBoundX;
				minBoundY = (minBoundY > mesh->GetMinY()) ? mesh->GetMinY() : minBoundY;
				minBoundZ = (minBoundZ > mesh->GetMinZ()) ? mesh->GetMinZ() : minBoundZ;


			}
		}
	}
	LoadMaterials(model);
	App->WriteIntoLog(INFO_LOG, " %s Loaded", filePath);
}

void MyModel::LoadMaterials(const tinygltf::Model& srcModel)
{
	size_t numTex = 0;
	for (const auto& srcMaterial : srcModel.materials)
	{  
		App->WriteIntoLog(INFO_LOG, "Loading texture no. %i", numTex);
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const Image& image = srcModel.images[texture.source];
			
			const char* imageUri = image.uri.c_str();
			Textures.push_back((App->GeTexture()->LoadTexture(imageUri)));
			numTex++;
		}
		
		
	}
}

void MyModel::Draw()
{
	for (int i = Meshes.size() - 1; i >= 0; i--) {
		Meshes[i]->Draw(Textures);
	}
}
