#pragma once

#include "ECS.h"

class Scene
{
public:
	Scene() {}

	~Scene() {}

	virtual void Load();

	virtual void CleanUp();

public:
	ECS::World* world = nullptr;
};

class MapScene : Scene
{
public:
	MapScene(const char* _filename) : Scene()
	{
		filename = _filename;
	}

	void Load();

	void CleanUp();

private:
	const char* filename;
};