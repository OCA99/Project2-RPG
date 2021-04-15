#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "ModuleUI.h"
#include "Scene.h"
#include "Input.h"
#include "Audio.h"
#include "GuiManager.h"
#include "Fonts.h"

ModuleUI::ModuleUI() : Module()
{
	name.Create("ui");
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Awake(pugi::xml_node& config)
{
	bool ret = true;



	return ret;
}

bool ModuleUI::Start()
{
	bool ret = true;


	return ret;
}

// Update: draw background
bool ModuleUI::Update(float dt)
{


	return true;
}

bool ModuleUI::PostUpdate()
{
	
	return true;
}

bool ModuleUI::CleanUp()
{

	bool ret = true;

	return ret;
}

void ModuleUI::UnLoad(int fontId)
{

}

void ModuleUI::IntToDynamicString(char* buffer, int k, int len)
{

	for (int i = 0; i < len; i++)
	{
		buffer[i] = '0';
	}

	buffer[len] = 0;

	int i = len - 1;
	while (k != 0)
	{
		if (i < 0) break;
		buffer[i--] += k % 10;
		k /= 10;
	}
}

void ModuleUI::IntToString(char* buffer, int k, int length)
{

	for (int i = 0; i < length; i++)
	{
		buffer[i] = '0';
	}

	buffer[length] = 0;

	int i = length - 1;
	while (k != 0)
	{
		if (i < 0) break;
		buffer[i--] += k % 10;
		k /= 10;
	}
}