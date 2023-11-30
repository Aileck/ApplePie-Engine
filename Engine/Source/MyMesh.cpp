#include "MyMesh.h"
#include "SDL.h"
#include "GL\glew.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "./include/MathGeoLib/Math/MathAll.h"

MyMesh::MyMesh()
{
}

MyMesh::~MyMesh()
{
}

void MyMesh::InitializeMesh()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glBindVertexArray(0);
}


void MyMesh::InitializeSeparatedArrayMesh()  // good for static meshes
{
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));

	//glBindVertexArray(0);
}

void MyMesh::InitializeInterleavedArraysMesh() //  good for dynamic meshes 
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));

	glBindVertexArray(0);
}

void MyMesh::LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	LoadVAO();
	LoadVBOTest(model, mesh, primitive);
	//LoadVBO(model, mesh, primitive);
	LoadEBO(model, mesh, primitive);
}

void MyMesh::LoadVAO() 
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));
	glBindVertexArray(0);
}
// Create Vertex Array Object (VAO)
void MyMesh::LoadVBO(const Model& model, const Mesh& mesh, const Primitive& primitive)
{
	const auto& itPos = primitive.attributes.find("POSITION");
	if (itPos != primitive.attributes.end())
	{
		// itPos->firt = label,  itPos->second = index
		const Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);

		// Which .bin file?
		const BufferView& posView = model.bufferViews[posAcc.bufferView];

		// Acess to .bin file
		const Buffer& posBuffer = model.buffers[posView.buffer];

		// Read .bin start point 
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		// Create VBO and assign memories
		glGenBuffers(1, &vbo); // 2
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Pass VTX data to GPU
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);

		// GPU(VBO) to CPU
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		//void* data = (glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		// Create verteix data
		for (size_t i = 0; i < posAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += posView.byteStride;
		}
		ptr[0] = float3 (0.0, 0.0, 0.0);
		ptr[1] = float3(1.0, 0.0, 0.0);
		ptr[2] = float3(0.0, 1.0, 0.0);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		// Set local variables
		vertexCount = posAcc.count;

		int materialIndex = primitive.material;
		/*LOG("%zu %i", model.materials.size(), materialIndex);*/
		if (materialIndex >= 0) {
			const Material& material = model.materials.at(materialIndex);
			textureID = material.pbrMetallicRoughness.baseColorTexture.index;
			enableTexture = true;
		}

	}
}void MyMesh::LoadVBOTest(const Model& model, const Mesh& mesh, const Primitive& primitive)
{
	const auto& itPos = primitive.attributes.find("POSITION");
	if (itPos != primitive.attributes.end())
	{
		const Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);

		// Which .bin file?
		const BufferView& posView = model.bufferViews[posAcc.bufferView];

		// Access to .bin file
		const Buffer& posBuffer = model.buffers[posView.buffer];

		// Read .bin start point
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		// Create VBO and assign memories
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Create vertex dat
		// Pass VTX data to GPU
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);

		// GPU(VBO) to CPU
		float* ptr = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		size_t ptrSize = 0;

		for (size_t i = 0; i < posAcc.count; ++i)
		{
			// Copy each component of the position vector
			for (int j = 0; j < 3; ++j)
			{
				ptr[i * 3 + j] = *reinterpret_cast<const float*>(bufferPos);
				bufferPos += sizeof(float);
				ptrSize++;
			}
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);

		// Create texture data
		glBufferData(GL_TEXTURE_BUFFER, sizeof(float) * 2 * posAcc.count, nullptr, GL_STATIC_DRAW);

		const unsigned char* bufferPosTexture = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);
		float* ptrTexture = reinterpret_cast<float*>(glMapBuffer(GL_TEXTURE_BUFFER, GL_WRITE_ONLY));

		for (size_t i = 0; i < posAcc.count; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				ptrTexture[i * 2 + j] = *reinterpret_cast<const float*>(bufferPos);
				bufferPosTexture += sizeof(float) + 12;
			}
		}
		glUnmapBuffer(GL_TEXTURE_BUFFER);

		// Set local variables
		vertexCount = posAcc.count;

		int materialIndex = primitive.material;
		/*LOG("%zu %i", model.materials.size(), materialIndex);*/
		if (materialIndex >= 0) {
			const Material& material = model.materials.at(materialIndex);
			textureID = material.pbrMetallicRoughness.baseColorTexture.index;
			enableTexture = true;
		}

	}
}

// Load index data from GLTF model
void MyMesh::LoadEBO(const Model& model, const Mesh& mesh, const Primitive& primitive)
{
	if (primitive.indices >= 0)
	{
		enableEBO = true;
		const Accessor& indAcc = model.accessors[primitive.indices];
		const BufferView& indView = model.bufferViews[indAcc.bufferView];

		// Set local variables
		indexCount = indAcc.count;

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset +
			indView.byteOffset]);

		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (auto i = 0; i < indexCount; ++i)
				ptr[i] = bufferInd[i];
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const uint16_t* bufferInd = reinterpret_cast<const uint16_t*>(buffer);
			for (auto i = 0; i < indexCount; ++i)
				ptr[i] = static_cast<unsigned int>(bufferInd[i]);
			
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			const uint8_t* bufferInd = reinterpret_cast<const uint8_t*>(buffer);
			for (auto i = 0; i < indexCount; ++i)
				ptr[i] = static_cast<unsigned int>(bufferInd[i]);
		}
		
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);


	}
}

//void MyMesh::Render()
//{
//	glUseProgram(App->GetProgram()->program);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));
//	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
//}

// Render with EBO
//void MyMesh::Render()
//{
//	glUseProgram(PROGRAM);
//	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));
//	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
//}

void MyMesh::RenderInterleavedArrays() // good for static meshes
{
	//glUseProgram(PROGRAM);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//IF EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	//IF EBO
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

}

void MyMesh::RenderSeparatedArrays() //  good for dynamic meshes writing
{
	//glUseProgram(PROGRAM);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));

	////IF EBO
	//glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void MyMesh::Draw(const std::vector<unsigned>& textures)
{
	// Verteix shader
	float4x4 view = App->GetCamera()->camera->ViewMatrix();
	float4x4 proj = App->GetCamera()->camera->ProjectionMatrix();
	glUseProgram(App->GetProgram()->program);

	glUniformMatrix4fv(0, 1, GL_TRUE, &modelMatrix[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);
	
	// Fragment shader
	if (enableTexture) {
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
	}

	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	if (enableEBO) {
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

}