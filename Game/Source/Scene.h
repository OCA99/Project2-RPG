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
		MAP
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

	~MapScene() {
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
