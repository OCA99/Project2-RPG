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
#include "CollisionSystem.h"
#include "NPCMovementSystem.h"

#include "PlayerFactory.h"
#include "NPCFactory.h"

void Scene::Load()
{
	world = ECS::World::createWorld();
}

void Scene::CleanUp()
{
	if (world != nullptr)
		world->destroyWorld();
}

void MapScene::Load(fPoint playerPosition)
{
	Scene::Load();

	world->registerSystem(new SpriteSystem());
	world->registerSystem(new AnimatorSystem());
	world->registerSystem(new PositionSystem());
	world->registerSystem(new HumanoidSystem());
	world->registerSystem(new CollisionSystem());
	world->registerSystem(new InputSystem());
	world->registerSystem(new CameraSystem());
	world->registerSystem(new NPCMovementSystem());

	PlayerFactory::Create(world, playerPosition);
	NPCFactory::Create(world, fPoint(250, 270));

	app->map->Load(filename);
}

void MapScene::CleanUp()
{
	Scene::CleanUp();
}