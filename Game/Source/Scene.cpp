#include "Scene.h"
#include "App.h"
#include "Map.h"

void MapScene::Load()
{
	app->map->Load(filename);
}

void MapScene::CleanUp()
{
	app->map->CleanUp();
}