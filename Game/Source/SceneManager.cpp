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
	MapScene* s = new MapScene("Town.tmx");
	LoadScene(s, fPoint(30, 250));


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
	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate(float dt)
{
	bool ret = true;

	currentScene->world->tick(dt);


	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void SceneManager::LoadScene(Scene* scene)
{
	delete currentScene;
	currentScene = scene;
	currentScene->Load();
}

void SceneManager::LoadScene(MapScene* scene, fPoint playerPosition)
{
	delete currentScene;
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
	LoadScene((Scene*)newS);
	
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