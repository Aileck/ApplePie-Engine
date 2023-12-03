#include "ModuleRenderExercise.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "SDL.h"
#include "GL\glew.h"
#include "./include/MathGeoLib/Geometry/Frustum.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

// Destructor
ModuleRenderExercise::~ModuleRenderExercise()
{
	CleanUp();
}

// Called before render is available
bool ModuleRenderExercise::Init()
{
	triangleVBO = CreateTriangleVBO();
	
	// Enable texture
	unsigned texture = App->GeTexture()->LoadTexture("Test-image-Baboon.ppm");

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture);
	return true;
}

// Called every draw update
update_status ModuleRenderExercise::Update()
{
	RenderVBO(triangleVBO);
	return UPDATE_CONTINUE;
}

unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	float vtx_data[] = {
			 0.5f,  0.5f, 0.0f,        
			 0.5f, -0.5f, 0.0f,         
			-0.5f, -0.5f, 0.0f,       
			-0.5f,  0.5f, 0.0f,         

			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
	};

	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	glObjectLabel(GL_BUFFER, vbo, -1, "Texture square");
	return vbo;
}

// This function must be called one time at destruction of vertex buffer
void ModuleRenderExercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
	//glDeleteTextures(1, *currentTexture);
}

// This function must be called each frame for drawing the triangle
void ModuleRenderExercise::RenderVBO(unsigned vbo)
{
	//float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
	//									float4x4::RotateZ(pi / 4.0f),
	//									float3(2.0f, 1.0f, 1.0f)
	//								);
	float4x4 model = float4x4::identity;

	float4x4 view = App->GetCamera()->camera->ViewMatrix();
	float4x4 proj = App->GetCamera()->camera->ProjectionMatrix();
	glUseProgram(App->GetProgram()->program);
	// Set the values of the matrix uniform variables
	
	glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);


	// bind buffer and vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// UV coordinate attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(12 * sizeof(float)));  
	
	// 1 triangle to draw = 3 vertices

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);



	//delete currentTexture;

}

float4x4 ModuleRenderExercise::LookAt(float3 target, float3 eye, float3 up) {

	float4x4 matrix;

	math::float3 f(target - eye); f.Normalize();
	math::float3 s(f.Cross(up)); s.Normalize();
	math::float3 u(s.Cross(f));
	matrix[0][0] = s.x; matrix[0][1] = s.y; matrix[0][2] = s.z;
	matrix[1][0] = u.x; matrix[1][1] = u.y; matrix[1][2] = u.z;
	matrix[2][0] = -f.x; matrix[2][1] = -f.y; matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(eye); matrix[1][3] = -u.Dot(eye); matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;

	return matrix;
}

bool ModuleRenderExercise::CleanUp()
{
	DestroyVBO(triangleVBO);
	return true;
}