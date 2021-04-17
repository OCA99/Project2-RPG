#include "GuiManager.h"

#include "App.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "Textures.h"
#include "Audio.h"
#include "Debug.h"

GuiManager::GuiManager() : Module()
{
	name.Create("guimanager");
}

GuiManager::~GuiManager()
{}

bool GuiManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool GuiManager::Start()
{
	return true;
}

bool GuiManager::Update(float dt)
{
	return UpdateAll(dt);

	return true;
}

bool GuiManager::PostUpdate(float dt)
{
	if (app->debug->bounds)
		DrawAll();

	return true;
}

bool GuiManager::CleanUp()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, SDL_Rect bounds, int id)
{
	GuiControl* control = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(id, bounds);
		control->SetObserver(app->scene);
		break;
	case GuiControlType::CHECKBOX:
		control = new GuiCheckBox(id, bounds);
		control->SetObserver(app->scene);
		break;
	case GuiControlType::SLIDER:
		control = new GuiSlider(id, bounds);
		control->SetObserver(app->scene);//preguntar EDU que onda
		break;

	default: break;
	}

	// Created entities are added to the list
	if (control != nullptr) controls.Add(control);

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	int i = controls.Find(entity);
	ListItem<GuiControl*>* c = controls.At(i);
	controls.Del(c);
}

void GuiManager::DestroyAllGuiControls()
{
	int u = controls.Count();

	for (int i = 0; i < u; i++)
	{
		delete controls.At(0)->data;
		controls.Del(controls.At(0));
	}
}

void GuiManager::AddGuiControl(GuiControl* entity)
{
	if (entity != nullptr) controls.Add(entity);
}

bool GuiManager::UpdateAll(float dt)
{
	for (int i = 0; i < controls.Count(); i++)
	{
		if (!controls[i]->Update(app->input, dt))
			return false;
	}

	return true;
}

void GuiManager::DrawAll()
{
	for (int i = 0; i < controls.Count(); i++)
	{
		controls[i]->Draw(app->render);
	}
}