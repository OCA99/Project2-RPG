#pragma once

#include "ECS.h"
#include "Point.h"
#include "App.h"
#include "Map.h"

class Scene
{
public:
	Scene() {}

	~Scene() {
		if (world != nullptr)
		{
			world->destroyWorld();
			world = nullptr;
		}
	}

	virtual void Load();

	virtual void CleanUp();

	enum class TYPE {
		MAP,
		LOGO,
		MENU,
		BATTLE
	};

public:
	ECS::World* world = nullptr;
	TYPE type;
};

class MapScene : Scene
{
public:
	MapScene(const char* _filename) : Scene()
	{
		filename = _filename;
		type = TYPE::MAP;
	}

	~MapScene()
	{
		app->map->CleanUp();

		if (world != nullptr)
		{
			world->destroyWorld();
			world = nullptr;
		}
	}

	void Load(fPoint playerPosition);

	void CleanUp();

	const char* filename;
};

class LogoScene : Scene
{
public:
	LogoScene() : Scene()
	{
		type = TYPE::LOGO;
	}

	~LogoScene()
	{
		app->tex->UnLoad(logoTexture);

		if (world != nullptr)
		{
			world->destroyWorld();
			world = nullptr;
		}
	}

	void Load();

	SDL_Texture** logoTexture;
};

class MenuScene : Scene
{
public:
	MenuScene() : Scene()
	{
		type = TYPE::MENU;
	}

	~MenuScene()
	{
		app->tex->UnLoad(mainMenu);
		app->tex->UnLoad(gameTitle);

		if (world != nullptr)
		{
			world->destroyWorld();
			world = nullptr;
		}
	}

	SDL_Texture** mainMenu;
	SDL_Texture** gameTitle;

	void Load();
};

class BattleScene : Scene
{
public:
	BattleScene() : Scene()
	{
		type = TYPE::BATTLE;
	}

	~BattleScene()
	{
		app->tex->UnLoad(background);

		if (world != nullptr)
		{
			world->destroyWorld();
			world = nullptr;
		}
	}

	SDL_Texture** background;

	void Load();
};
