#pragma once
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "./include/tinygltf/tiny_gltf.h"

using namespace tinygltf;
class MyMesh
{
public:
	MyMesh();
	~MyMesh();

	void CreateVAO();

	void LoadVAO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void Render();
	void RenderInterleavedArrays();
	void RenderSeparatedArrays();
	void Draw(const std::vector<unsigned>& textures);
private:
	unsigned m_vbo = 0;
	unsigned m_vao = 0;
	unsigned m_ebo = 0;
	size_t vertexCount = 0;
	size_t indexCount = 0;

};