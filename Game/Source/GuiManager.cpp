#include "GuiManager.h"

#include "App.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "Textures.h"
#include "Audio.h"
#include "Debug.h"
#include "Fonts.h"
#include "Easing.h"

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
	menuButtonTex = app->tex->Load("Textures/UI/MainPauseMenu/menu_button.png");
	optionsButtonsTex = app->tex->Load("Textures/UI/OptionsMenu/options_buttons.png");
	invButtonTex = app->tex->Load("Textures/UI/HUD/inventory_button.png");
	
	sliderTex = app->tex->Load("Textures/UI/OptionsMenu/volume_slider.png");
	backButtonTex = app->tex->Load("Textures/UI/OptionsMenu/back_button.png");
	checkBoxTex = app->tex->Load("Textures/UI/OptionsMenu/checkbox.png");

	startTex = app->tex->Load("Textures/UI/MainPauseMenu/start.png");
	continueTex = app->tex->Load("Textures/UI/MainPauseMenu/continue.png");
	optionsTex = app->tex->Load("Textures/UI/MainPauseMenu/options.png");
	exitTex = app->tex->Load("Textures/UI/MainPauseMenu/exit.png");
	saveTex = app->tex->Load("Textures/UI/MainPauseMenu/save.png");
	loadTex = app->tex->Load("Textures/UI/MainPauseMenu/load.png");
	mainMenuTex = app->tex->Load("Textures/UI/MainPauseMenu/main_menu_text.png");
	graphicsTex = app->tex->Load("Textures/UI/OptionsMenu/graphics.png");
	audioTex = app->tex->Load("Textures/UI/OptionsMenu/audio.png");
	controlsTex = app->tex->Load("Textures/UI/OptionsMenu/controls.png");
	onOffTex = app->tex->Load("Textures/UI/OptionsMenu/on_off.png");
	backArrowTex = app->tex->Load("Textures/UI/OptionsMenu/back_arrow.png");
	questButtonTex = app->tex->Load("Textures/UI/HUD/quest_button.png");
	pixelTex = app->tex->Load("Textures/UI/OptionsMenu/pixel.png");
	itemTex = app->tex->Load("Textures/UI/OptionsMenu/item_action.png");
	useItemText = app->tex->Load("Textures/UI/OptionsMenu/useitem.png");
	discardItemText = app->tex->Load("Textures/UI/OptionsMenu/discarditem.png");
	cancelItemText = app->tex->Load("Textures/UI/OptionsMenu/cancel.png");
	puzzlePassTex = app->tex->Load("Textures/Puzzles/white_button.png");
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

	/*int fps = 1.0f / dt;

	std::string s = std::to_string(fps);
	app->fonts->BlitText(10, 10, 0, s.c_str());*/

	return true;
}

bool GuiManager::CleanUp()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, SDL_Rect bounds, int id, bool anim, int fPos, int time, SplineType esType, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int sliderValue)
{
	GuiControl* control = nullptr;
	SDL_Texture** textTex = nullptr;
	SDL_Texture** buttonTex = nullptr;

	switch (id)
	{
	case 0:
		textTex = startTex;
		buttonTex = menuButtonTex;
		break;
	case 1:
		buttonTex = menuButtonTex;
		textTex = continueTex;
		break;
	case 2:
		buttonTex = menuButtonTex;
		textTex = optionsTex;
		break;
	case 3:
		buttonTex = menuButtonTex;
		textTex = exitTex;
		break;
	case 4:
		buttonTex = menuButtonTex;
		textTex = continueTex;
		break;
	case 5:
		buttonTex = menuButtonTex;
		textTex = saveTex;
		break;
	case 6:
		buttonTex = menuButtonTex;
		textTex = loadTex;
		break;
	case 7:
		buttonTex = menuButtonTex;
		textTex = mainMenuTex;
		break;
	case 11:
		buttonTex = optionsButtonsTex;
		textTex = graphicsTex;
		break;
	case 12:
		buttonTex = optionsButtonsTex;
		textTex = audioTex;
		break;
	case 13:
		buttonTex = optionsButtonsTex;
		textTex = controlsTex;
		break;
	case 14:
		buttonTex = backButtonTex;
		textTex = backArrowTex;
		break;
	case 17:
		buttonTex = invButtonTex;
		textTex = pixelTex;
		break;
	case 18:
		buttonTex = questButtonTex;
		textTex = pixelTex;
		break;
	case 19: //Use Item
		buttonTex = itemTex;
		textTex = useItemText;
		break;
	case 20://Discard Item
		buttonTex = itemTex;
		textTex = cancelItemText;
		break;
	case 21://Puzzle Button
		buttonTex = puzzlePassTex;
		textTex = pixelTex;
		break;
	case 22://Inv Party Button
		buttonTex = pixelTex;
		textTex = pixelTex;
		break;
	case 23://Remove Item 
		buttonTex = itemTex;
		textTex = discardItemText;
		break;
	default:
		break;
	}

	//8,9,10 are volume sliders

	switch (type)
	{
	case GuiControlType::BUTTON:
		if (anim == true)
		{
			control = new GuiButton(id,  buttonTex, bounds, textTex,fPos,time,esType);
			control->SetObserver(app->scene);
			control->color.r = r;
			control->color.g = g;
			control->color.b = b;
			control->color.a = a;
			if (id == 1)
			{
				type = type;
			}

		} else if (anim == false) 
		{
			control = new GuiButton(id, bounds, buttonTex, textTex);
			control->SetObserver(app->scene);
			control->color.r = r;
			control->color.g = g;
			control->color.b = b;
			control->color.a = a;
			if (id == 1)
			{
				type = type;
			}
		}
		
		break;
	case GuiControlType::CHECKBOX:
		control = new GuiCheckBox(id, bounds, checkBoxTex, onOffTex);
		control->SetObserver(app->scene);
		control->color.r = r;
		control->color.g = g;
		control->color.b = b;
		control->color.a = a;
		break;
	case GuiControlType::SLIDER:
		control = new GuiSlider(id, bounds, sliderTex, sliderValue);
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
		if (controls.At(0)->data->id != 17 && controls.At(0)->data->id != 18)
		{
		delete controls.At(0)->data;
		controls.Del(controls.At(0));

		}

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