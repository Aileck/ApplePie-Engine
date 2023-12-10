#pragma once
#include "Module.h"
#include "MyModel.h"

class LoadModelModule :
    public Module
{
public:
	bool Init();

	update_status PreUpdate();

	update_status Update();

	update_status PostUpdate();

	bool CleanUp();
private:
	MyModel m_model;
};

