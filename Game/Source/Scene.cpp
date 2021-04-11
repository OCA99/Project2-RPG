#include "Scene.h"
#include "App.h"
#include "Map.h"
#include "Textures.h"

#include "SpriteSystem.h"
#include "AnimatorSystem.h"
#include "InputSystem.h"
#include "PositionSystem.h"
#include "HumanoidSystem.h"
#include "CameraSystem.h"

#include "PlayerFactory.h"

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

	world->registerSystem(new SpriteSystem());
	world->registerSystem(new AnimatorSystem());
	world->registerSystem(new InputSystem());
	world->registerSystem(new PositionSystem());
	world->registerSystem(new HumanoidSystem());
	world->registerSystem(new CameraSystem());

	PlayerFactory::Create(world, fPoint(10, 10));

	app->map->Load(filename);
}

void MapScene::CleanUp()
{
	Scene::CleanUp();
	app->map->CleanUp();
}