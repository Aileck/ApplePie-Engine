#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleInput.h"
#include "ModuleRenderExercise.h"
#include "ModuleLoadModel.h"


#include "./GL/glew.h"
#include "SDL.h"
#include "Application.h"

const float CAMERA_MOV_SPEED = 0.005f;
const float CAMERA_ROT_SPEED = math::pi / 3600.0f;
const float CAMERA_MOV_BOOST = 2.0f;
//const float CAMERA_MOV_SPEED = 0.1f;
//const float CAMERA_ROT_SPEED = -math::pi / 360.0f;
//const float CAMERA_MOV_BOOST = 20.0f;
ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{

}

float4x4 LookAt(float3 target, float3 eye, float3 up) {
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

bool ModuleCamera::Init()
{
	camera = new Frustum();

	camera->type = FrustumType::PerspectiveFrustum;
	camera->pos = float3(0.0f, 2.0f, -8.0f);

	camera->nearPlaneDistance = 0.1f;
	camera->farPlaneDistance = 2100.0f;
	camera->verticalFov = math::pi / 4.0f;
	camera->horizontalFov = 2.f * atanf(tanf(camera->verticalFov * 0.5f) * (App->GetWindow()->currentWidth / App->GetWindow()->currentHeight));

	float4x4 view = LookAt(float3(0.0f, 0.0f, 0.0f), camera->pos, float3::unitY);
	camera->front = -view.WorldZ();
	camera->up = view.WorldY();
	initialView = camera->ViewMatrix();
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	HandleMovement();
	HandleRotation();
	HandleFocus();

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::HandleMovement()
{
	float finalSpeed = CAMERA_MOV_SPEED;

	// FPS-like movement
	if (App->GetInput()->CheckIfMouseDown(SDL_BUTTON_RIGHT)) {
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_LSHIFT)) {
			finalSpeed *= CAMERA_MOV_BOOST;
		}

		// UP - DOWN
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_Q)) {
			//ModifyCameraPosY(0.001f);
			ModifyCameraPosY(finalSpeed);
		}
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_E)) {
			//ModifyCameraPosY(-0.001f);
			ModifyCameraPosY(-finalSpeed);
		}

		// GO FOWARD - BACK
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_W)) {
			//ModifyCameraPosY(0.001f);
			ModifyCameraPosZ(finalSpeed);
		}
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_S)) {
			//ModifyCameraPosY(-0.001f);
			ModifyCameraPosZ(-finalSpeed);
		}

		// GO LEFT - RIGHT
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_A)) {
			//ModifyCameraPosY(0.001f);
			ModifyCameraPosX(-finalSpeed);
		}
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_D)) {
			//ModifyCameraPosY(-0.001f);
			ModifyCameraPosX(finalSpeed);
		}
	}

	//ZOOM
	else if (App->GetInput()->GetMouseWheelValue() != 0) {
		ModifyCameraPosZ(finalSpeed * CAMERA_MOV_BOOST * App->GetInput()->GetMouseWheelValue());
	}
}

void ModuleCamera::HandleRotation()
{
	if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_LEFT)) {
		float3x3 rotationDeltaMatrix = float3x3::RotateY(CAMERA_ROT_SPEED); // = some rotation delta value
		float3 oldFront = camera->front.Normalized();
		camera->front = rotationDeltaMatrix * oldFront;
		float3 oldUp = camera->up.Normalized();
		camera->up = rotationDeltaMatrix * oldUp;
		//float4x4 newView = LookAt(float3(camera->pos.x + , camera->pos.y, camera->pos.z), camera->pos, float3::unitY);
		//camera->front = -newView.WorldZ();
		//camera->up = newView.WorldY();
	}
	if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_RIGHT)) {
		float3x3 rotationDeltaMatrix = float3x3::RotateY(-CAMERA_ROT_SPEED); // = some rotation delta value
		float3 oldFront = camera->front.Normalized();
		camera->front = rotationDeltaMatrix * oldFront;
		float3 oldUp = camera->up.Normalized();
		camera->up = rotationDeltaMatrix * oldUp;
	}

}

void ModuleCamera::HandleFocus()
{
	if (App->GetModelLoader()->GetModel() != nullptr) {

		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_F)) {
			//ModifyCameraPosX(-finalSpeed);
			FocusOn(float3(0.0f, 0.0f, 0.0f));
		}
	}
}

void ModuleCamera::FocusOn(float3 target)
{
	float4x4 view = LookAt(target, camera->pos, float3::unitY);
	camera->front = -view.WorldZ();
	camera->up = view.WorldY();
}

void ModuleCamera::ModifyCameraPosX(float X)
{
	camera->Translate(X * Cross(camera->front, camera->up));
}

void ModuleCamera::ModifyCameraPosY(float Y)
{
	camera->Translate(float3(0.0, Y,0.0));
}

void ModuleCamera::ModifyCameraPosZ(float Z)
{
	camera->Translate(Z * camera->front);
}

float4x4 ModuleCamera::LookAt(float3 target, float3 eye, float3 up) {

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

//void 