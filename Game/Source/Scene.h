#pragma once

#include "ECS.h"
#include "Point.h"

class Scene
{
public:
	Scene() {}

	~Scene() {}

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

	void Load(fPoint playerPosition);

	void CleanUp();

	const char* filename;
};
