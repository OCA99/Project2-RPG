#include "Scene.h"
#include "App.h"
#include "Map.h"
#include "Textures.h"
#include "Audio.h"

#include "SpriteSystem.h"
#include "AnimatorSystem.h"
#include "InputSystem.h"
#include "PositionSystem.h"
#include "HumanoidSystem.h"
#include "CameraSystem.h"
#include "CollisionSystem.h"
#include "NPCMovementSystem.h"
#include "LogoFadeSystem.h"

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

	app->map->Load(filename);
}

void MapScene::CleanUp()
{
	Scene::CleanUp();
}

void LogoScene::Load()
{
	Scene::Load();

	world->registerSystem(new SpriteSystem());
	world->registerSystem(new LogoFadeSystem());

	ECS::Entity* e = world->create();

	e->assign<Position>(fPoint(150, 50));

	app->audio->LoadFx("Assets/Audio/Fx/logo.ogg");//0
	SDL_Texture* t = app->tex->Load("Assets/Textures/logo.png");

	e->assign<Sprite>(t, 0.2f);

	e = world->create();
	e->assign<LogoFade>(70);
}
