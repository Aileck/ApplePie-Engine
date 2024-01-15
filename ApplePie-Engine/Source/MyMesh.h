#pragma once
#include "./include/tinygltf/tiny_gltf.h"
#include "./include/MathGeoLib/Math/MathAll.h"

class MyTexture;
class MyMaterial;

using namespace tinygltf;
class MyMesh
{
public:
	MyMesh();
	~MyMesh();

	void LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Draw();

	void SetMaterial(MyMaterial* material) { mMaterial = material; }

	inline int GetVerticesCount() const { return  vertexCount; }
	inline int GetIndexCount() const { return  indexCount; }
	inline int GetTextureID() const { return  textureID; }
	inline MyMaterial* GetMaterial() const { return  mMaterial; }

	inline float3 GetMaxXYZ() const { return maxXYZ; }
	inline float3 GetMinXYZ() const { return minXYZ; }

	inline float4x4 GetMeshTransform() const {return modelMatrix;}

private:
	void LoadVAO();
	void LoadVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	MyMaterial* mMaterial = nullptr;
	unsigned vbo = 0;
	unsigned vao = 0;
	unsigned ebo = 0;
	
	size_t vertexCount = 0;
	size_t indexCount = 0;
	unsigned textureID = 0;

	// Draw rule
	bool enablePosition = false;
	bool enableTexture = false;
	bool enableEBO = false;

	float3 maxXYZ;
	float3 minXYZ;

	// MVP
	//float4x4 modelMatrix = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
	//									float4x4::RotateZ(pi / 4.0f),
	//									float3(2.0f, 1.0f, 1.0f)
	//								);
	float4x4 modelMatrix = float4x4::identity;
};