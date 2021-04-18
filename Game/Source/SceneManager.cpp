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

#include "Defs.h"
#include "Log.h"

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
	menuTex = app->tex->Load("Assets/Textures/UI/PauseMenu/pause_menu.png");

	pauseTex = app->tex->Load("Assets/Textures/UI/PauseMenu/pause_text.png");
	continueTex = app->tex->Load("Assets/Textures/UI/PauseMenu/continue_text.png");
	saveTex = app->tex->Load("Assets/Textures/UI/PauseMenu/save_text.png");
	loadTex = app->tex->Load("Assets/Textures/UI/PauseMenu/load_text.png");
	optionTex = app->tex->Load("Assets/Textures/UI/PauseMenu/option_text.png");
	mainTex = app->tex->Load("Assets/Textures/UI/PauseMenu/mainmenu_text.png");

	app->audio->songToBeLoaded = "Assets/Audio/Music/Originals/town_main.wav";

	sceneToBeLoaded = (Scene*)s;
	playerPositionToBeLoaded = fPoint(30, 250);

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
		if (sceneToBeLoaded->type == Scene::TYPE::MAP)
			LoadScene((MapScene*)sceneToBeLoaded, playerPositionToBeLoaded);
		else
			LoadScene(sceneToBeLoaded);
		sceneToBeLoaded = nullptr;
	}

	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate(float dt)
{
	bool ret = true;

	currentScene->world->tick(dt);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		
		menu = !menu;
		
	}
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

	if (menu)
	{
		app->render->DrawTexture(menuTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		app->render->DrawTexture(pauseTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		app->render->DrawTexture(continueTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		app->render->DrawTexture(saveTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		app->render->DrawTexture(loadTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		app->render->DrawTexture(optionTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		app->render->DrawTexture(mainTex, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
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
	MapScene* s;
	switch (control->id)
	{
	case 0:
		app->ui->DestroyAllGuiControls();
		s = new MapScene("Town.tmx");
		app->scene->sceneToBeLoaded = (Scene*)s;
		break;
	case 1:
		app->ui->DestroyAllGuiControls();
		app->RequestLoad();
		break;
	case 3:
		return false;
		break;
	default:
		break;
	}

	return true;
}
