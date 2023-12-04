#pragma once
#include "./include/tinygltf/tiny_gltf.h"
#include "./include/MathGeoLib/Math/MathAll.h"

using namespace tinygltf;
class MyMesh
{
public:
	MyMesh();
	~MyMesh();

	/*void InitializeMesh();*/

	void InitializeSeparatedArrayMesh();

	void InitializeInterleavedArraysMesh();

	void LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadVAO();
	void LoadVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadVBOTest(const Model& model, const Mesh& mesh, const Primitive& primitive);
	void LoadPosition(const Model& model, const Mesh& mesh, const Primitive& primitive, const int index/*, float* ptr*/);
	void LoadPosition2(const Model& model, const int index, float* ptr, size_t& counter);
	void LoadTexcoord2(const Model& model, const int index, float* ptr, size_t& counter);
	void LoadTexcoord(const Model& model, const Mesh& mesh, const Primitive& primitive, const int index);
	
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	//void Render();
	void RenderInterleavedArrays();
	void RenderSeparatedArrays();
	void Draw(const std::vector<unsigned>& textures);
private:
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

	int numVerteixForBuffer = 0;



	// MVP
	//float4x4 modelMatrix = float4x4::identity;
	float4x4 modelMatrix = float4x4::FromTRS(float3(0.0f, 2.0f, 0.0f),
									float4x4::identity,
									float3(25.0f, 25.0f, 25.0f)
								);
};