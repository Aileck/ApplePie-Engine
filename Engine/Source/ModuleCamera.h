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

	void HandleMovement();

	void HandleRotation();
	//const getter to get camera
	
	
	//float4x4 float4x4

	Frustum* camera = nullptr;
	float4x4 initialView;

private:
	void ModifyCameraPosX(float X);
	void ModifyCameraPosY(float Y);
	void ModifyCameraPosZ(float Z);
	float4x4 LookAt(float3 target, float3 eye, float3 up);
};

