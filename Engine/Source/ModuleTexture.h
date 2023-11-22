#pragma once
#include "Module.h"
#include "Globals.h"

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

	//unsigned* LoadTexture(const char* textureName);
	unsigned LoadTexture(const char* textureName);

};

