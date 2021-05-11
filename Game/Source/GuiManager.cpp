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
	menuButtonTex = app->tex->Load("Assets/Textures/UI/MainPauseMenu/menu_button.png");
	startTex = app->tex->Load("Assets/Textures/UI/MainPauseMenu/start.png");
	continueTex = app->tex->Load("Assets/Textures/UI/MainPauseMenu/continue.png");
	optionsTex = app->tex->Load("Assets/Textures/UI/MainPauseMenu/options.png");
	exitTex = app->tex->Load("Assets/Textures/UI/MainPauseMenu/exit.png");
	saveTex = app->tex->Load("Assets/Textures/UI/MainPauseMenu/save.png");
	loadTex = app->tex->Load("Assets/Textures/UI/MainPauseMenu/load.png");
	mainMenuTex = app->tex->Load("Assets/Textures/UI/MainPauseMenu/main_menu_text.png");

	return true;
}

bool GuiManager::Update(float dt)
{
	return UpdateAll(dt);

	return true;
}

bool GuiManager::PostUpdate(float dt)
{
		DrawAll();

	return true;
}

bool GuiManager::CleanUp()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, SDL_Rect bounds, int id, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	GuiControl* control = nullptr;
	SDL_Texture* textTex = nullptr;
	switch (id)
	{
	case 0:
		textTex = startTex;
		break;
	case 1:
		textTex = continueTex;
		break;
	case 2:
		textTex = optionsTex;
		break;
	case 3:
		textTex = exitTex;
		break;
	case 4:
		textTex = continueTex;
		break;
	case 5:
		textTex = saveTex;
		break;
	case 6:
		textTex = loadTex;
		break;
	case 7:
		textTex = mainMenuTex;
		break;
	default:
		break;
	}

	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(id, bounds, menuButtonTex, textTex);
		control->SetObserver(app->scene);
		control->color.r = r;
		control->color.g = g;
		control->color.b = b;
		control->color.a = a;
		break;
	case GuiControlType::CHECKBOX:
		control = new GuiCheckBox(id, bounds);
		control->SetObserver(app->scene);
		control->color.r = r;
		control->color.g = g;
		control->color.b = b;
		control->color.a = a;
		break;
	case GuiControlType::SLIDER:
		control = new GuiSlider(id, bounds);
		control->SetObserver(app->scene);//preguntar EDU que onda
		control->color.r = r;
		control->color.g = g;
		control->color.b = b;
		control->color.a = a;
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