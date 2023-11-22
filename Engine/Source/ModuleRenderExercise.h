#ifndef __ModuleRenderExercise_H__
#define __ModuleRenderExercise_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"
#include "./include/MathGeoLib/Math/MathAll.h"

class Application;

class ModuleRenderExercise :public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();
	bool Init();
	update_status Update();

	unsigned triangleVBO;
	unsigned* currentTexture;

	unsigned CreateTriangleVBO();
	void DestroyVBO(unsigned vbo);
	void RenderVBO(unsigned vbo);
	float4x4 LookAt(float3 target, float3 eye, float3 up);

	bool CleanUp();

	//math::float4x4 model;
	//math::float4x4 view;
	//math::float4x4 proj;
};


#endif // __ModuleRenderExercise_H__