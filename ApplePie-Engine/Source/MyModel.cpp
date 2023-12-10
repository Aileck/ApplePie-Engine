#include "Globals.h"
#include "MyModel.h"
#include "Application.h"
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
	App->WriteIntoLog(INFO_LOG, "Deleting the current model");
	for (MyMesh* mesh : Meshes)
	{
		delete mesh;
	}

	for (MyTexture* texture : Textures)
	{
		delete texture;
	}

	Meshes.clear();
	Textures.clear();
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
				maxBoundX = (maxBoundX < mesh->GetMaxXYZ().x) ? mesh->GetMaxXYZ().x : maxBoundX;
				maxBoundY = (maxBoundY < mesh->GetMaxXYZ().y) ? mesh->GetMaxXYZ().y : maxBoundY;
				maxBoundZ = (maxBoundZ < mesh->GetMaxXYZ().z) ? mesh->GetMaxXYZ().z : maxBoundZ;

				minBoundX = (minBoundX > mesh->GetMinXYZ().x) ? mesh->GetMinXYZ().x : minBoundX;
				minBoundY = (minBoundY > mesh->GetMinXYZ().y) ? mesh->GetMinXYZ().y : minBoundY;
				minBoundZ = (minBoundZ > mesh->GetMinXYZ().z) ? mesh->GetMinXYZ().z : minBoundZ;
			}
		}
	}
	LoadMaterials(model);
	ComputeCenterOfModel();
	App->WriteIntoLog(INFO_LOG, " %s Loaded", filePath);

	delete[] filePath;
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

			MyTexture* myTexture = new MyTexture();
			myTexture->LoadTexture(imageUri);
			Textures.push_back(myTexture);
			numTex++;
		}	
	}
}

void MyModel::LoadExternalTexture(const char* assetFileName)
{
	// Delete texture
	for (MyTexture* texture : Textures)
	{
		delete texture;
	}

	Textures.clear();

	MyTexture* myTexture = new MyTexture();
	myTexture->LoadTexture(assetFileName);
	Textures.push_back(myTexture);
}

void MyModel::ComputeCenterOfModel()
{
	float3 minBound = float3(minBoundX, minBoundY, minBoundZ);
	float3 maxBound = float3(maxBoundX, maxBoundY, maxBoundZ);
	centerModel = (minBound + maxBound) * 0.5f;
}


void MyModel::Draw() const
{
	for (int i = 0; i < Meshes.size(); i++) {
		Meshes[i]->Draw(Textures);
	}
}

