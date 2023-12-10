#pragma once
#include "Module.h"
#include "MyModel.h"

#include "./include/MathGeoLib/Math/MathAll.h"
class ModuleLoadModel :
    public Module
{
public:
	ModuleLoadModel();
	~ModuleLoadModel();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void LoadExteriorModel(const char* fullpath);
	void LoadExteriorTexture(const char* fullpath);

	inline MyModel* GetModel() const {return model;}
	inline float3 GetCenter() const {return sceneCenter;}
private:

	MyModel* model;
	float3 sceneCenter;
};

