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
	LoadScene(s);

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

	/*currentScene->world->all([](ECS::Entity* ent) 
	{
		ECS::ComponentHandle<Position> position = ent->get<Position>();
		if (position.isValid())
		{
			position->position.x;
		}
		// do something with ent
	});*/

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		app->RequestSave();
		LOG("hola");
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
	currentScene = scene;
	currentScene->Load();
}

void SceneManager::LoadScene(MapScene* scene)
{
	LoadScene((Scene*)scene);
}

bool SceneManager::Load(pugi::xml_node& savedGame)
{
	//passar la escena a LoadScene
	/*
	pugi::xml_node sceneNode = savedGame.child("currentScene");
	SString string = sceneNode.attribute("name").as_string();
	
	MapScene* s = new MapScene(string);
	LoadScene(s);
	*/
	return true;
}

bool SceneManager::Save(pugi::xml_node& savedGame)
{
	LOG("scene saved");
	pugi::xml_node sceneNode = savedGame.append_child("currentScene");
	pugi::xml_attribute currentSceneAtt = sceneNode.append_attribute("name");

	currentSceneAtt.set_value(currentScene);
	
	//guardar entitats etc
	return true;
}