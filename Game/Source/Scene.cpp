#include "Scene.h"
#include "App.h"
#include "Map.h"
#include "SpriteRenderer.h"
#include "PlayerFactory.h"

#include "Textures.h"

void Scene::Load()
{
	world = ECS::World::createWorld();
}

void Scene::CleanUp()
{
	world->destroyWorld();
}

void MapScene::Load()
{
	Scene::Load();

	world->registerSystem(new SpriteRenderer());

	PlayerFactory::Create(world, fPoint(10, 10));
	PlayerFactory::Create(world, fPoint(60, 60));

	app->map->Load(filename);
}

void MapScene::CleanUp()
{
	Scene::CleanUp();
	app->map->CleanUp();
}