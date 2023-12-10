#pragma once
#include "Module.h"
#include "Globals.h"
#include "./include/MathGeoLib/Geometry/Frustum.h"
#include "./include/MathGeoLib/Math/MathAll.h"
#include "./include/MathGeoLib/Math/MathConstants.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	inline Frustum* GetCamera() { return camera; };

	void SetCameraPosX(float X) const { camera->pos.x = X; };
	void SetCameraPosY(float Y) const { camera->pos.y = Y; };
	void SetCameraPosZ(float Z) const { camera->pos.z = Z; };
	
	void SetUpX(float X) const { camera->up.x = X; };
	void SetUpY(float Y) const { camera->up.y = Y; };
	void SetUpZ(float Z) const { camera->up.z = Z; };

	void SetFrontX(float X) const { camera->front.x = X; };
	void SetFrontY(float Y) const { camera->front.y = Y; };
	void SetFrontZ(float Z) const { camera->front.z = Z; };


	//const getter to get camera
	
	
	//float4x4 float4x4

	
	float4x4 initialView;

private:
	void ModifyCameraPosX(float X);
	void ModifyCameraPosY(float Y);
	void ModifyCameraPosZ(float Z);
	float4x4 LookAt(float3 target, float3 eye, float3 up);

	void HandleMovement();
	void HandleRotation();
	void HandleFocus();
	void FocusOn(float3 target);
	void OrbitOn(float3 target);
	Frustum* camera = nullptr;

};

