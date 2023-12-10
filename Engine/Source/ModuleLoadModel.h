#pragma once
#include "Module.h"
#include "MyModel.h"

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
	void AdjustCameraPosition();

	inline MyModel* GetModel() const {return model;}
private:
	MyModel* model;
};

