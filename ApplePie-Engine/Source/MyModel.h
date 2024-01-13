#pragma once
#include "Application.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "./include/tinygltf/tiny_gltf.h"

#include "MyMesh.h"
#include "MyMaterial.h"
#include <vector>
#include <limits>

class MyTexture;

using namespace tinygltf;
class MyModel
{
public:
	MyModel();
	MyModel(const char* assetFileName, bool exterior = false);

	~MyModel();

	void Load(const char* assetFileName, bool exterior = false);

	void LoadExternalTexture(const char* assetFileName);
	void Draw() const;

	inline std::vector<MyMesh*> GetMeshes() const { return Meshes; }
	inline std::vector<MyTexture*> GetTextures() const { return Textures; }

	inline float GetMaxX() const { return maxBoundX; }
	inline float GetMaxY() const { return maxBoundY; }
	inline float GetMaxZ() const { return maxBoundZ; }

	inline float GetMinX() const { return minBoundX; }
	inline float GetMinY() const { return minBoundY; }
	inline float GetMinZ() const { return minBoundZ; }
	inline float3 GetCenter() const { return centerModel; }

private:

	void LoadMaterials(const tinygltf::Model& srcModel);
	void ComputeCenterOfModel();

	std::vector<MyMesh*> Meshes;
	std::vector<MyTexture*> Textures;
	std::vector <MyMaterial*> Materiales;

	float maxBoundX = -std::numeric_limits<float>::infinity();
	float maxBoundY = -std::numeric_limits<float>::infinity();
	float maxBoundZ = -std::numeric_limits<float>::infinity();
	
	float minBoundX = std::numeric_limits<float>::infinity();
	float minBoundY = std::numeric_limits<float>::infinity();
	float minBoundZ = std::numeric_limits<float>::infinity();

	float3 centerModel;
};

