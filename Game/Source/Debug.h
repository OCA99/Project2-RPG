#pragma once

#include "Module.h"

#include "Point.h"

#include "SDL/include/SDL.h"

class Debug : public Module
{
public:

	Debug();

	// Destructor
	virtual ~Debug();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

public:
	bool godMode = false;
	bool bounds = false;
};