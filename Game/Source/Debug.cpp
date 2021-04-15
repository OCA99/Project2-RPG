#include "Debug.h"

#include "App.h"
#include "Input.h"

Debug::Debug()
{
}

Debug::~Debug()
{
}

bool Debug::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
	}

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		bounds = !bounds;
	}

	return true;
}

bool Debug::CleanUp()
{
	return true;
}
