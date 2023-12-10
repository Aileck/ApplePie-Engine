#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleInput.h"
#include "ModuleLoadModel.h"
#include "ModuleTimer.h"


#include "./GL/glew.h"
#include "SDL.h"
#include "Application.h"

const float CAMERA_MOV_SPEED = 5.0f;
const float CAMERA_ROT_SPEED = math::pi / 2.0f;
const float CAMERA_MOV_BOOST = 2.0f;

#define DELTA_TIME (App->GetTimer()->GetDeltaTime())

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
	
}

bool ModuleCamera::Init()
{
	camera = new Frustum();

	camera->type = FrustumType::PerspectiveFrustum;
	camera->pos = float3(0.0f, 2.0f, 8.0f);

	camera->nearPlaneDistance = 0.1f;
	camera->farPlaneDistance = 2100.0f;
	camera->verticalFov = math::pi / 4.0f;
	camera->horizontalFov = 2.f * atanf(tanf(camera->verticalFov * 0.5f) * (App->GetWindow()->GetCurrentWidth() / App->GetWindow()->GetCurrentHeight()));

	float4x4 view = LookAt(float3(0.0f, 2.0f, 0.0f), camera->pos, float3::unitY);
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
	delete camera;
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
			ModifyCameraPosY(finalSpeed);
		}
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_E)) {
			ModifyCameraPosY(-finalSpeed);
		}

		// GO FOWARD - BACK
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_W)) {
			ModifyCameraPosZ(finalSpeed);
		}
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_S)) {
			ModifyCameraPosZ(-finalSpeed);
		}

		// GO LEFT - RIGHT
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_A)) {
			ModifyCameraPosX(-finalSpeed);
		}
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_D)) {
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
		float3x3 rotationDeltaMatrix = float3x3::RotateY(CAMERA_ROT_SPEED * DELTA_TIME); 
		float3 oldFront = camera->front.Normalized();
		camera->front = rotationDeltaMatrix * oldFront;
		float3 oldUp = camera->up.Normalized();
		camera->up = rotationDeltaMatrix * oldUp;

	}
	if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_RIGHT)) {
		float3x3 rotationDeltaMatrix = float3x3::RotateY(-CAMERA_ROT_SPEED * DELTA_TIME); 
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
			FocusOn(App->GetModelLoader()->GetModel()->GetCenter());
		}
		if (App->GetInput()->CheckIfPressed(SDL_SCANCODE_LALT)) {
			if(App->GetInput()->GetDragDistance() !=  0)
			OrbitOn(App->GetModelLoader()->GetModel()->GetCenter());
		}
	}
}

void ModuleCamera::FocusOn(float3 target)
{
	MyModel* model = (App->GetModelLoader()->GetModel());
	float3 maxBound = float3(model->GetMaxX(), model->GetMaxY(), model->GetMaxZ());
	float distance = target.Distance(maxBound);

	float3 newCameraPos = target + float3(0.0f, 0.0f, distance * 4);
	App->GetCamera()->SetCameraPosX(newCameraPos.x);
	App->GetCamera()->SetCameraPosY(newCameraPos.y);
	App->GetCamera()->SetCameraPosZ(newCameraPos.z);
}

void ModuleCamera::OrbitOn(float3 target)
{
	float rotationAngle = CAMERA_ROT_SPEED * App->GetInput()->GetDragDistance() * 0.01 * DELTA_TIME;
	float3 toCenter = target - camera->pos;

	Quat rotationQuat = Quat::RotateAxisAngle(float3::unitY, rotationAngle);

	toCenter = rotationQuat * toCenter;

	camera->pos = target - toCenter;
	camera->front = (target - camera->pos).Normalized();
	camera->up = float3::unitY;
}

void ModuleCamera::ModifyCameraPosX(float X)
{
	camera->Translate(X * Cross(camera->front, camera->up) * DELTA_TIME);
}

void ModuleCamera::ModifyCameraPosY(float Y)
{
	camera->Translate(Y * camera->up * DELTA_TIME);
}

void ModuleCamera::ModifyCameraPosZ(float Z)
{
	camera->Translate(Z * camera->front * DELTA_TIME);
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
