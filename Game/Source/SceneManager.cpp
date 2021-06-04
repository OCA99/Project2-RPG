#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ECS.h"
#include "Components.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "QuestManager.h"
#include "BattleManager.h"
#include "ItemManager.h"
#include "Audio.h"
#include "SceneTransitionSystem.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "ItemManager.h"

#include <Windows.h>
#include <iostream>
#include <conio.h>
using namespace std;

#include "Defs.h"
#include "Log.h"


#include "SDL_mixer/include/SDL_mixer.h"


SceneManager::SceneManager() : Module()
{
	name.Create("scene");
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	//MapScene* s = new MapScene("Town.tmx");

	LogoScene* s = new LogoScene();
	menuTex = app->tex->Load("Textures/UI/MainPauseMenu/pause_menu.png");
	optionsTex = app->tex->Load("Textures/UI/OptionsMenu/options_menu.png");
	controlsMenuTex = app->tex->Load("Textures/UI/OptionsMenu/controls_menu.png");
	questMenuTex = app->tex->Load("Textures/UI/HUD/quest_menu.png");
	invMenu = app->tex->Load("Textures/UI/HUD/charactermenu.png");

	audioMenuTex = app->tex->Load("Textures/UI/OptionsMenu/audio_menu.png");
	graphicsMenuTex = app->tex->Load("Textures/UI/OptionsMenu/graphics_menu.png");

	app->audio->songToBeLoaded = "Audio/Music/Originals/town_main.wav";

	sceneToBeLoaded = (Scene*)s;
	playerPositionToBeLoaded = fPoint(30, 250);

	startPressed = true;

	POINT m;
	GetCursorPos(&m);

	mousePosition.x = m.x;
	mousePosition.y = m.y;

	return true;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	if (sceneToBeLoaded != nullptr)
	{
		
		if (currentScene != nullptr && currentScene->type == Scene::TYPE::MAP)
		{
			if (isFinished == false)
			{
				alpha += speed * dt;


				if (alpha >= 255)
				{
					isFinished = true;
				}


				if (app->volume > 0 && app->volumeUp == false)
				{
					app->volume -= volSpeed * dt;
					app->volumeDown = true;

				}
				//LOG("%f", app->volume);
				if (app->volume == 0.0f) 
				{
					app->volume = 0.0f;
					app->volumeDown = false;
				}

				app->UpdateVolume();
			}
			else
			{
				if (sceneToBeLoaded->type == Scene::TYPE::MAP)
					LoadScene((MapScene*)sceneToBeLoaded, playerPositionToBeLoaded);
				else
					LoadScene(sceneToBeLoaded);
				sceneToBeLoaded = nullptr;
				isFinished = false;
				alpha = 0;
			}
		}
		else
		{
			if (sceneToBeLoaded->type == Scene::TYPE::MAP)
				LoadScene((MapScene*)sceneToBeLoaded, playerPositionToBeLoaded);
			else
				LoadScene(sceneToBeLoaded);
			sceneToBeLoaded = nullptr;
			isFinished = false;

		}

	}

	changingScene = false;

	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate(float dt)
{
	bool ret = true;
	GamePad& pad = app->input->pads[0];

	currentScene->world->tick(dt);

	SDL_Rect fullscreen;
	fullscreen.x = 0;
	fullscreen.y = 0;
	uint w, h;
	app->win->GetWindowSize(w, h);
	fullscreen.w = w;
	fullscreen.h = h;

	int squares = 16;
	int squareSize = fullscreen.w / squares;

	for (int i = 0; i < squares; i++) {
		for (int j = 0; j < squares; j++) {
			SDL_Rect rect;
			float prop = min(alpha, 255.0f) / 255.0f;
			if ((i - j) % 2 == 0)
				rect = SDL_Rect({ i * squareSize, j * squareSize, squareSize, int(std::round(squareSize * prop)) });
			else
				rect = SDL_Rect({ i * squareSize, j * squareSize + int(std::round(squareSize * (1 - prop))), squareSize, int(std::round(squareSize * prop)) });
			app->render->DrawRectangle(rect, 0, 0, 0, 255, true, false);
		}
	}

	if ((app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.start == true) && !optionsMenu && app->dialog->currentDialog == nullptr && !app->battle->isBattling && !app->quests->questInvOpened && !app->items->invOpened && startPressed)
	{
		app->audio->PlayFx(8, 0);
		if(startPressed)menu = !menu;
		startPressed = false;
	}

	if (pad.b == true && bPressed)
	{
		if (menu && !optionsMenu && app->dialog->currentDialog == nullptr && !app->battle->isBattling && !app->quests->questInvOpened && !app->items->invOpened)
		{
			app->audio->PlayFx(8, 0);
			menu = false;
			bPressed = false;
		}
	}

	if (pad.start == false) startPressed = true;
	if (pad.b == false) bPressed = true;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		app->RequestSave();
		LOG("saving");
	}

	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		app->RequestLoad();
		LOG("loading");
	}
	if (menu || currentScene->type == Scene::TYPE::MENU || optionsMenu || app->quests->questInvOpened || app->items->invOpened) {
		

		float dtSpeed = padSpeed * dt;

		LOG("padx: %f pady: %f", pad.l_x, pad.l_y);

		if (pad.l_x > 0.6f || pad.right)
		{
			SetCursorPos(mousePosition.x, mousePosition.y);
			mousePosition.x += dtSpeed;
		}
		if (pad.l_x < -0.6f || pad.left)
		{
			SetCursorPos(mousePosition.x, mousePosition.y);
			mousePosition.x -= dtSpeed;
		}
		if (pad.l_y < -0.6f || pad.up)
		{
			SetCursorPos(mousePosition.x, mousePosition.y);
			mousePosition.y -= dtSpeed;
		}
		if (pad.l_y > 0.6f || pad.down)
		{
			SetCursorPos(mousePosition.x, mousePosition.y);
			mousePosition.y += dtSpeed;
		}
	}

	if (currentScene->type == Scene::TYPE::MENU && !optionsMenu && !app->quests->questInvOpened)
	{
		if (!buttons)
		{
			app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 148, 120, 32 }), 0); //start
			app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 201, 120, 32 }), 1); //continue
			app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 256, 120, 32 }), 2); //options
			app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 311, 120, 32 }), 3); //exit
			buttons = true;
			padSpeed = 400;
		}
	}
	
	if (currentScene->type != Scene::TYPE::MENU && currentScene->type != Scene::TYPE::LOGO && currentScene->type != Scene::TYPE::BATTLE) {

		padSpeed = 600;

		if (app->quests->questInvOpened) app->render->DrawTexture(questMenuTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		if (app->items->invOpened) app->render->DrawTexture(invMenu, 0, 0, &SDL_Rect({ 0,0,1280,720 }), 0.5f, 1, 0, 0, 0, false);

		if (menu)
		{

			if (!optionsMenu)
			{
				app->render->DrawTexture(menuTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);

				if (buttons == false)
				{
					app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 260, 82, 120, 32 }), 4);//continue
					app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 260, 271 / 2, 120, 32 }), 5);//save
					app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 260, 381 / 2, 120, 32 }), 6);//load
					app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 260, 491 / 2, 120, 32 }), 2);//options
					app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 260, 601 / 2, 120, 32 }), 7);//back to menu
					buttons = true;
				}

				if (app->volume > 25)
				{
					app->volumeDown = true;
					app->volume -= volSpeed * 1.25f * dt;
				}

				if (app->volume < 25)
				{
					app->volumeDown = false;
					app->volume = 25;
				}

				Mix_VolumeMusic(app->volume);
			}
		
		}

		if (menu == false && !app->quests->questInvOpened)
		{
			if (app->volume < 100 && app->volumeDown == false)
			{
				app->volumeUp = true;
				app->volume += volSpeed * dt;
			}
			if (app->volume <= 0.0f)
			{
				app->volumeDown = false;
			}
			if (app->volume > 100)
			{
				app->volume = 100;
				app->volumeUp = false;
			}


			Mix_VolumeMusic(app->volume);
			if(buttons)app->ui->DestroyAllGuiControls();
			buttons = false;
		}
	}

	if(optionsMenu)
	{
		if (!pad.r1) r1Pressed = true;
		if (!pad.l1) l1Pressed = true;

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || (pad.b && bPressed))
		{
			app->audio->PlayFx(6, 0);
			optionsMenu = false;
			audioSelected = false;
			controlsSelected = false;
			graphicsSelected = false;
			app->ui->DestroyAllGuiControls();
			bPressed = false;
			buttons = false;
		}
		app->render->DrawTexture(optionsTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);

		if (graphicsSelected)
		{
			app->render->DrawTexture(graphicsMenuTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);

			if (buttons == false)
			{

				app->ui->CreateGuiControl(GuiControlType::CHECKBOX, SDL_Rect({ 728 / 2, 323 / 2, 183 / 2, 50 / 2 }), 15); //fullscreen checkbox
				GuiControl* c = app->ui->CreateGuiControl(GuiControlType::CHECKBOX, SDL_Rect({ 728 / 2, 462 / 2, 183 / 2, 50 / 2 }), 16); //vsync checkbox
				((GuiCheckBox*)c)->checked = true;
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 140 / 2, 152 / 2, 340 / 2, 65 / 2 }), 11); //graphics button
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 468 / 2, 152 / 2, 340 / 2, 65 / 2 }), 12); //audio button
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 798 / 2, 152 / 2, 340 / 2, 65 / 2 }), 13); //controls button
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 58 / 2, 34 / 2, 60 / 2, 60 / 2 }), 14); //back button

				buttons = true;
			}
			if (pad.r1 && r1Pressed)
			{
				app->ui->DestroyAllGuiControls();
				audioSelected = true;
				graphicsSelected = false;
				controlsSelected = false;
				buttons = false;
				r1Pressed = false;
			}
			if (pad.l1 && l1Pressed)
			{
				app->ui->DestroyAllGuiControls();
				audioSelected = false;
				graphicsSelected = false;
				controlsSelected = true;
				buttons = false;
				l1Pressed = false;
			}
		}

		if (audioSelected)
		{
			app->render->DrawTexture(audioMenuTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);

			if (buttons == false)
			{
				app->ui->CreateGuiControl(GuiControlType::SLIDER, SDL_Rect({ 715 / 2, 330 / 2, 300 / 2, 30 / 2 }), 8); //general volume slider
				app->ui->CreateGuiControl(GuiControlType::SLIDER, SDL_Rect({ 715 / 2, 412 / 2, 300 / 2, 30 / 2 }), 9); //music volume slider
				app->ui->CreateGuiControl(GuiControlType::SLIDER, SDL_Rect({ 715 / 2, 494 / 2, 300 / 2, 30 / 2 }), 10); //fx volume slider
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 140 / 2, 152 / 2, 340 / 2, 65 / 2 }), 11); //graphics button
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 468 / 2, 152 / 2, 340 / 2, 65 / 2 }), 12); //audio button
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 798 / 2, 152 / 2, 340 / 2, 65 / 2 }), 13); //controls button
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 58 / 2, 34 / 2, 60 / 2, 60 / 2 }), 14); //back button

				buttons = true;
			}
			if (pad.r1 == true && r1Pressed)
			{
				app->ui->DestroyAllGuiControls();
				audioSelected = false;
				graphicsSelected = false;
				controlsSelected = true;
				buttons = false;
				r1Pressed = false;
			}
			if (pad.l1 && l1Pressed)
			{
				app->ui->DestroyAllGuiControls();
				audioSelected = false;
				graphicsSelected = true;
				controlsSelected = false;
				buttons = false;
				l1Pressed = false;
			}
		}

		if (controlsSelected)
		{
			app->render->DrawTexture(controlsMenuTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);

			if (buttons == false)
			{
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 140 / 2, 152 / 2, 340 / 2, 65 / 2 }), 11); //graphics button
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 468 / 2, 152 / 2, 340 / 2, 65 / 2 }), 12); //audio button
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 798 / 2, 152 / 2, 340 / 2, 65 / 2 }), 13); //controls button
				app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 58 / 2, 34 / 2, 60 / 2, 60 / 2 }), 14); //back button

				buttons = true;
			}
			if (pad.l1 == true && l1Pressed)
			{
				app->ui->DestroyAllGuiControls();
				audioSelected = true;
				graphicsSelected = false;
				controlsSelected = false;
				buttons = false;
				l1Pressed = false;
			}
			if (pad.r1 && r1Pressed)
			{
				app->ui->DestroyAllGuiControls();
				audioSelected = false;
				graphicsSelected = true;
				controlsSelected = false;
				buttons = false;
				r1Pressed = false;
			}
		}

	}
	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	if (currentScene != nullptr && currentScene->type == Scene::TYPE::MAP)
		delete (MapScene*)currentScene;
	else
		delete currentScene;

	return true;
}

void SceneManager::LoadScene(Scene* scene)
{
	CleanUp();
	currentScene = scene;
	currentScene->Load();
}

void SceneManager::LoadScene(MapScene* scene, fPoint playerPosition)
{
	CleanUp();
	currentScene = (Scene*)scene;
	scene->Load(playerPosition);
}

bool SceneManager::Load(pugi::xml_node& savedGame)
{
	//passar la escena a LoadScene
	
	pugi::xml_node sceneNode = savedGame.child("currentScene");
	const char* string = sceneNode.attribute("name").as_string();
	
	pugi::xml_node entityNode = savedGame.child("entity");
	pugi::xml_node positionNode = entityNode.child("position");

	MapScene* newS = new MapScene(string);

	sceneToBeLoaded = (Scene*)newS;
	playerPositionToBeLoaded = fPoint(positionNode.attribute("x").as_int(), positionNode.attribute("y").as_int());
	
	currentScene->world->all([&](ECS::Entity* ent)
	{
		ECS::ComponentHandle<Position> position = ent->get<Position>();
		if (position.isValid())
		{
			position->position.x = positionNode.attribute("x").as_int();
			position->position.y = positionNode.attribute("y").as_int();
		}
			// do something with ent
	});

	return true;
}

bool SceneManager::Save(pugi::xml_node& savedGame)
{
	LOG("scene saved");
	pugi::xml_node sceneNode = savedGame.append_child("currentScene");
	pugi::xml_attribute currentSceneAtt = sceneNode.append_attribute("name");

	if (currentScene->type == Scene::TYPE::MAP)
	{
		MapScene* mapScene = (MapScene*)currentScene;
		currentSceneAtt.set_value(mapScene->filename);
	}

	pugi::xml_node entityNode = savedGame.append_child("entity");
	pugi::xml_node positionNode = entityNode.append_child("position");

	currentScene->world->all([&](ECS::Entity* ent)
	{
		ECS::ComponentHandle<Position> position = ent->get<Position>();
		if (position.isValid())
		{
			pugi::xml_attribute positionEntityx = positionNode.append_attribute("x");
			positionEntityx.set_value(position->position.x);
			pugi::xml_attribute positionEntityy = positionNode.append_attribute("y");
			positionEntityy.set_value(position->position.y);
		}
		// do something with ent
	});
	//guardar entitats etc
	return true;
}

bool SceneManager::OnGuiMouseClickEvent(GuiControl* control)
{
	Scene* s;
	GuiSlider* slider;

	switch (control->id)
	{
	case 0: //start button
		app->ui->DestroyAllGuiControls();
		s = (Scene*)(new MapScene("Town.tmx"));
		app->scene->sceneToBeLoaded = s;
		app->scene->playerPositionToBeLoaded = fPoint(30, 250);
		break;
	case 1: //continue button
		app->ui->DestroyAllGuiControls();
		app->RequestLoad();
		app->scene->menu = 0;
		break;
	case 2: //options button
		app->ui->DestroyAllGuiControls();
		buttons = false;
		optionsMenu = true;
		audioSelected = true;
		break;
	case 3: //exit
		return false;
		break;
	case 4: //continue (exit menu) button
		app->ui->DestroyAllGuiControls();
		menu = 0;

		break;
	case 5: //save
		app->RequestSave();
		break;
	case 6: //load
		app->ui->DestroyAllGuiControls();
		app->RequestLoad();
		app->scene->menu = 0;
		break;
	case 7: //back to menu
		app->ui->DestroyAllGuiControls();
		s = (Scene*)(new MenuScene());
		sceneToBeLoaded = s;
		menu = false;
		break;
	case 8:
		slider = (GuiSlider*)control;
		app->volume = slider->value;
		app->UpdateVolume();
		break;
	case 9:
		slider = (GuiSlider*)control;
		app->volumeMusic = slider->value;
		app->UpdateVolume();
		break;
	case 10:
		slider = (GuiSlider*)control;
		app->volumeFx = slider->value;
		app->UpdateVolume();
		break;
	case 11: //graphics window button
		app->ui->DestroyAllGuiControls();
		graphicsSelected = true;
		audioSelected = false;
		controlsSelected = false;
		buttons = false;
		break;
	case 12: //audio window button
		app->ui->DestroyAllGuiControls();
		audioSelected = true;
		graphicsSelected = false;
		controlsSelected = false;
		buttons = false;
		break;
	case 13: //controls window button
		app->ui->DestroyAllGuiControls();
		controlsSelected = true;
		audioSelected = false;
		graphicsSelected = false;
		buttons = false;
		break;
	case 14: //back button
		app->ui->DestroyAllGuiControls();
		optionsMenu = false;
		audioSelected = false;
		controlsSelected = false;
		graphicsSelected = false;
		buttons = false;
		app->items->invOpened = false;
		app->quests->questInvOpened = false;
		break;
	case 15: //fullscreen checkbox
		app->win->ToggleFullscreen();
		break;
	case 16:
		app->render->SwitchVsync();
		break;
	case 19:
		//app->items->useItem = true;
		break;
	case 20:
		break;

	default:
		break;
	}
	//8,9,10 are audio sliders

	return true;
}
