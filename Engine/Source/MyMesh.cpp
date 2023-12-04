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

void MyMesh::LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	LoadVBO(model, mesh, primitive);
	LoadEBO(model, mesh, primitive);
	LoadVAO();
}

void MyMesh::LoadVAO() 
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void MyMesh::LoadVBO(const Model& model, const Mesh& mesh, const Primitive& primitive)
{
	const auto& itPos = primitive.attributes.find("POSITION");
	const Accessor& posAcc = model.accessors[itPos->second];
	vertexCount += posAcc.count;
	enablePosition = itPos != primitive.attributes.end();

	const auto& itUV = primitive.attributes.find("TEXCOORD_0");
	enableTexture = itUV != primitive.attributes.end();

	numVerteixForBuffer = (enablePosition * 3) + (enableTexture * 2);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Pass VTX data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVerteixForBuffer * posAcc.count, nullptr, GL_STATIC_DRAW);

	// GPU(VBO) to CPU
	float* ptr = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	size_t ptrPos = 0;

	if(enablePosition)
		LoadPosition2(model, itPos->second, ptr, ptrPos);

	if(enableTexture)
		LoadTexcoord2(model,  itUV->second, ptr, ptrPos);

	glUnmapBuffer(GL_ARRAY_BUFFER);

}

void MyMesh::LoadPosition2(const Model& model, const int index, float* ptr, size_t& pos)
{
	const Accessor& posAcc = model.accessors[index];
	SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
	SDL_assert(posAcc.componentType == GL_FLOAT);

	// Which .bin file?
	const BufferView& posView = model.bufferViews[posAcc.bufferView];

	// Access to .bin file
	const Buffer& posBuffer = model.buffers[posView.buffer];

	// Read .bin start point
	const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

	for (size_t i = pos; i < posAcc.count; ++i)
	{
		// Copy each component of the position vector
		for (int j = 0; j < 3; ++j)
		{
			ptr[i * 3 + j] = *reinterpret_cast<const float*>(bufferPos);
			//bufferPos += sizeof(float);
			//if (posView.byteStride == 0) {
			//	bufferPos += posView.byteStride;
			//}
			//else {
				bufferPos += sizeof(float);
			//}
			
			pos++;
		}
	}
	

}

void MyMesh::LoadTexcoord2(const Model& model, const int index, float* ptr, size_t& pos)
{
	const Accessor& uvAcc = model.accessors[index];
	const BufferView& uvView = model.bufferViews[uvAcc.bufferView];

	// Access to .bin file
	const Buffer& uvBuffer = model.buffers[uvView.buffer];

	// Read .bin start point
	const unsigned char* bufferUV = &(uvBuffer.data[uvAcc.byteOffset + uvView.byteOffset]);

	size_t posCopy = pos;
	for (size_t i = (posCopy /2); i < uvAcc.count + posCopy; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			// Copy each component of the texcoord vector
			ptr[i * 2 + j] = *reinterpret_cast<const float*>(bufferUV);
			//if (uvView.byteStride == 0) {
			//	bufferUV += uvView.byteStride;
			//}
			//else {
				bufferUV += sizeof(float);
			//}
			pos++;
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
			for (auto i = 0; i < indexCount; ++i) {
				ptr[i] = (bufferInd[i]);
				LOG("%u\n", (unsigned int)bufferInd[i]);
			}
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