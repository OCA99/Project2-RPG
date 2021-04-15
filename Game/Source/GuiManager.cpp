#include "GuiManager.h"

#include "App.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "Textures.h"
#include "Audio.h"

GuiManager::GuiManager() : Module()
{
	name.Create("guimanager");
}

GuiManager::~GuiManager()
{}

bool GuiManager::Awake(pugi::xml_node& config)
{
	pugi::xml_node guimanagerPathN = config.child("guimanager");

	checkBoxPath = guimanagerPathN.attribute("checkbox").as_string();
	arrowMenuPath = guimanagerPathN.attribute("menuArrow").as_string();
	sliderPath = guimanagerPathN.attribute("slider").as_string();

	pugi::xml_node audio = config.child("audio");

	pressButtonFxPath = audio.attribute("pressButton").as_string();
	hoverButtonFxPath = audio.attribute("hoverButton").as_string();
	checkboxFxPath = audio.attribute("checkboxButton").as_string();

	return true;
}

bool GuiManager::Start()
{

	checkBoxTex = app->tex->Load(checkBoxPath);
	arrowMenuTex = app->tex->Load(arrowMenuPath);
	sliderTex = app->tex->Load(sliderPath);

	hoverButtonFx = app->audio->LoadFx(hoverButtonFxPath);
	pressButtonFx = app->audio->LoadFx(pressButtonFxPath);
	checkboxFx = app->audio->LoadFx(checkboxFxPath);

	return true;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::PostUpdate()
{

	DrawAll();

	return true;
}

bool GuiManager::CleanUp()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int x, int y, SDL_Rect bounds, int id)
{
	GuiControl* control = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(id, bounds, arrowMenuPath);
		control->SetObserver(app->scene);
		break;
	case GuiControlType::CHECKBOX:
		control = new GuiCheckBox(id, bounds, checkBoxPath);
		control->SetObserver(app->scene);
		break;
	case GuiControlType::SLIDER:
		control = new GuiSlider(id, bounds, sliderPath);
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

void GuiManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		for (int i = 0; i < controls.Count(); i++)
		{
			controls[i]->Update(app->input, dt);
		}
	}
}

void GuiManager::DrawAll()
{
	for (int i = 0; i < controls.Count(); i++)
	{
		if (showDebug)
			controls[i]->DrawDebug(app->render);
		controls[i]->Draw(app->render);
	}
}