#include "MyMesh.h"
#include "SDL.h"
#include "GL\glew.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "MyTexture.h"
#include "MyMaterial.h"
#include "./include/tinygltf/tiny_gltf.h"
#include "./include/MathGeoLib/Math/MathAll.h"

MyMesh::MyMesh()
{
}

MyMesh::~MyMesh()
{
	// Release the VAO, VBO, and EBO
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	if (enableEBO) {
		glDeleteBuffers(1, &ebo);
	}
}

void MyMesh::LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	LoadVBO(model, mesh, primitive);
	LoadEBO(model, mesh, primitive);
	LoadVAO();
	App->WriteIntoLog(INFO_LOG, "Rendering Pipeline Setted");
}

void MyMesh::LoadVAO() 
{
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));

	glBindVertexArray(0);
}

void MyMesh::LoadVBO(const Model& model, const Mesh& mesh, const Primitive& primitive)
{
	// Create VBO and assign memories
	glGenBuffers(1, &vbo);

	unsigned VerteixDataSize = 0;

	// Load position
	const auto& itPos = primitive.attributes.find("POSITION");
	enablePosition = (itPos != primitive.attributes.end());
	if(enablePosition)
	{
		const Accessor& posAcc = model.accessors[itPos->second];
		VerteixDataSize += sizeof(float) * 3 * posAcc.count;
	}
	else {
		App->WriteIntoLog(ERROR_LOG, "Error loading model");
	}

	// Load tex coord
	const auto& itUV = primitive.attributes.find("TEXCOORD_0");
	enableTexture = (itUV != primitive.attributes.end());
	if(enableTexture)
	{
		const Accessor& uvAcc = model.accessors[itUV->second];
		VerteixDataSize += sizeof(float) * 2  * uvAcc.count;
	}

	//int VerteixDataBufferSize = ()
	// 1.Read Position
	// 2.IF has texture coordinate,read it
	if (enablePosition) {
		const Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		// Which .bin file?
		const BufferView& posView = model.bufferViews[posAcc.bufferView];
		// Access to .bin file
		const Buffer& posBuffer = model.buffers[posView.buffer];
		// Read .bin start point
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// Create vertex dat
		// Pass VTX data to GPU
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 7 * posAcc.count, nullptr, GL_STATIC_DRAW);
		// GPU(VBO) to CPU
		float3* ptr = reinterpret_cast<float3*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, VerteixDataSize, GL_MAP_WRITE_BIT));

		for (size_t i = 0; i < posAcc.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += sizeof(float) * 3;
		}

		unsigned offsetVBO = sizeof(float) * 3 * posAcc.count;
		glUnmapBuffer(GL_ARRAY_BUFFER);
		// Set local variables
		vertexCount += posAcc.count;

		// Get Verteix Max and Min
		float maxX = posAcc.maxValues[0];
		float maxY = posAcc.maxValues[1];
		float maxZ = posAcc.maxValues[2];

		float minX = posAcc.minValues[0];
		float minY = posAcc.minValues[1];
		float minZ = posAcc.minValues[2];

		maxXYZ = float3(maxX, maxY, maxZ);
		minXYZ = float3(minX, minY, minZ);

		maxXYZ = modelMatrix.MulPos(maxXYZ);
		minXYZ = modelMatrix.MulPos(minXYZ);

		App->WriteIntoLog(INFO_LOG, "Vertex Data Loaded");
		if (enableTexture) {
			const Accessor& uvAcc = model.accessors[itUV->second];
			SDL_assert(uvAcc.type == TINYGLTF_TYPE_VEC2);
			SDL_assert(uvAcc.componentType == GL_FLOAT);
			// Which .bin file?
			const BufferView& uvView = model.bufferViews[uvAcc.bufferView];
			// Access to .bin file
			const Buffer& uvBuffer = model.buffers[uvView.buffer];
			// Read .bin start point
			const unsigned char* bufferUV = &(uvBuffer.data[uvAcc.byteOffset + uvView.byteOffset]);

			float2* uvs = reinterpret_cast<float2*>(glMapBufferRange(GL_ARRAY_BUFFER, offsetVBO, sizeof(float) * 2 * uvAcc.count, GL_MAP_WRITE_BIT));
			for (size_t i = 0; i < uvAcc.count; ++i)
			{
				uvs[i] = *reinterpret_cast<const float2*>(bufferUV);
				bufferUV += sizeof(float) * 2;
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);

			// Load Texture index
			const Material& material = model.materials[primitive.material];
			const PbrMetallicRoughness& pbrMR = material.pbrMetallicRoughness;
			const TextureInfo& baseColorTextureInfo = pbrMR.baseColorTexture;
			int baseColorTextureIndex = baseColorTextureInfo.index;
			textureID = baseColorTextureInfo.texCoord;

			App->WriteIntoLog(INFO_LOG, "Texture Coordinates Loaded");
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

void MyMesh::Draw(MyMaterial* material)
{
	// Verteix shader
	float4x4 view = App->GetCamera()->GetCamera()->ViewMatrix();
	float4x4 proj = App->GetCamera()->GetCamera()->ProjectionMatrix();
	glUseProgram(App->GetProgram()->program);

	glUniformMatrix4fv(0, 1, GL_TRUE, &modelMatrix[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);

	// Fragment shader
	if (true) {
		//glActiveTexture(GL_TEXTURE5);
		//glBindTexture(GL_TEXTURE_2D, textures[textureID]->getTextureID());
		glUniform3fv(
			glGetUniformLocation(App->GetProgram()->program,
				"material.diffuseColor"), 1, &material->GetDiffuseFactor().xyz()[0]);

		glUniform3fv(
			glGetUniformLocation(App->GetProgram()->program,
				"material.specularColor"), 1, &material->GetSpecularFactor()[0]);

		glUniform1i(
			glGetUniformLocation(App->GetProgram()->program,
				"material.shininess"), material->GetGlossinessFactor());


		if (material->GetDiffuseMap() != nullptr)
		{
			glUniform1i(
				glGetUniformLocation(App->GetProgram()->program,
					"material.hasDiffuseMap"), true);
			GLint diffuseTextureLoc =
				glGetUniformLocation(App->GetProgram()->program, "material.diffuseTexture");
			glUniform1i(diffuseTextureLoc, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->GetDiffuseMap()->getTextureID());
		}
		else {
			glUniform1i(
				glGetUniformLocation(App->GetProgram()->program,
					"material.hasDiffuseMap"), false);
		}

		if (material->GetSpecularMap() != nullptr)
		{
			glUniform1i(
				glGetUniformLocation(App->GetProgram()->program,
					"material.hasSpecularMap"), true);
			GLint specularTextureLoc =
				glGetUniformLocation(App->GetProgram()->program, "material.specularTexture");
			glUniform1i(specularTextureLoc, 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, material->GetSpecularMap()->getTextureID());
		}
		else
		{
			glUniform1i(
				glGetUniformLocation(App->GetProgram()->program,
					"material.hasSpecularMap"), false);
		}

		// Light
		//uniform vec3 lightDir;
		//uniform vec3 lightColor;
		//uniform float lightIntensity;
		//vec3 material.ambientColor

	}

	glBindVertexArray(vao);
	if (enableEBO) {
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

}