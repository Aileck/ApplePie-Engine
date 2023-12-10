#pragma once
#include "Module.h"
#include "Globals.h"

class MyTexture;

class ModuleTexture :
    public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	MyTexture* LoadTexture(const char* textureName, const bool external = false);

};

