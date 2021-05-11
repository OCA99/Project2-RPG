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
#include "Audio.h"
#include "SceneTransitionSystem.h"

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
	menuTex = app->tex->Load("Assets/Textures/UI/MainPauseMenu/pause_menu.png");

	app->audio->songToBeLoaded = "Assets/Audio/Music/Originals/town_main.wav";

	sceneToBeLoaded = (Scene*)s;
	playerPositionToBeLoaded = fPoint(30, 250);

	startPressed = true;

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
				LOG("%f", app->volume);
				if (app->volume == 0.0f) 
				{
					app->volume = 0.0f;
					app->volumeDown = false;
				}

				Mix_VolumeMusic(app->volume);
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

	app->render->DrawRectangle(fullscreen, 0, 0, 0, std::min(int(alpha), 255), true, false);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.start == true)
	{
		
		if(startPressed)menu = !menu;
		startPressed = false;
	}

	if (pad.start == false) startPressed = true;
	//ret = false;

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

	if (currentScene->type != Scene::TYPE::MENU && currentScene->type != Scene::TYPE::LOGO && currentScene->type != Scene::TYPE::BATTLE) {

		if (menu)
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

			app->volume = 25;
			Mix_VolumeMusic(app->volume);

		}

		if (menu == false)
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
			app->ui->DestroyAllGuiControls();
			buttons = false;
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

	switch (control->id)
	{
	case 0:
		app->ui->DestroyAllGuiControls();
		s = (Scene*)(new MapScene("Town.tmx"));
		app->scene->sceneToBeLoaded = s;
		app->scene->playerPositionToBeLoaded = fPoint(30, 250);
		break;
	case 1:
		app->ui->DestroyAllGuiControls();
		app->RequestLoad();
		app->scene->menu = 0;
		break;
	case 3:
		return false;
		break;
	case 4:
		menu = 0;
		break;
	case 5:
		app->RequestSave();
		break;
	case 6:
		app->ui->DestroyAllGuiControls();
		app->RequestLoad();
		app->scene->menu = 0;
		break;
	case 7:
		app->ui->DestroyAllGuiControls();
		s = (Scene*)(new MenuScene());
		sceneToBeLoaded = s;
		menu = false;
		break;
	default:
		break;
	}

	return true;
}
