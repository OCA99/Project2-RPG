#include "Scene.h"
#include "App.h"
#include "Map.h"
#include "Textures.h"
#include "GuiManager.h"
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

	std::string s = filename;
	
	if (s == "Town.tmx")
	{
		app->audio->songToBeLoaded = "Assets/Audio/Music/Originals/town_main.wav";
		app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);
	}
	else if (s == "Tavern.tmx")
	{
		app->audio->songToBeLoaded = "Assets/Audio/Music/Originals/town_tavern.wav";
		app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);
	}
	else if (s == "Forest.tmx")
	{
		app->audio->songToBeLoaded = "Assets/Audio/Music/Originals/forest_main.wav";
		app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);
	}
	else if (s == "NPC House.tmx")
	{
		app->audio->songToBeLoaded = "Assets/Audio/Music/Originals/town_house.wav";
		app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);
	}
	else if (s == "NPC House.tmx")
	{
		app->audio->songToBeLoaded = "Assets/Audio/Music/Originals/town_house.wav";
		app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);
	}

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

	app->audio->LoadFx("Assets/Audio/Fx/logo.ogg");//1
	app->audio->LoadFx("Assets/Audio/Fx/action_interact.ogg");//2
	app->audio->LoadFx("Assets/Audio/Fx/title.ogg");//3
	app->audio->LoadFx("Assets/Audio/Fx/action_door.ogg");//4
	app->audio->LoadFx("Assets/Audio/Fx/player_footsteps_ground.ogg");//5

	SDL_Texture* t = app->tex->Load("Assets/Textures/logo.png");

	e->assign<Sprite>(t, 0.2f);

	e = world->create();
	e->assign<LogoFade>(70);
}

void MenuScene::Load()
{
	Scene::Load();

	world->registerSystem(new SpriteSystem());

	app->audio->songToBeLoaded = "Assets/Audio/Music/Originals/town_blacksmith.wav";
	app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);

	SDL_Texture* t = app->tex->Load("Assets/Textures/UI/MainMenu/main_menu.png");

	ECS::Entity* e = world->create();
	e->assign<Position>(fPoint(0, 0));
	e->assign<Sprite>(t, 0.5f);

	t = app->tex->Load("Assets/Textures/UI/MainMenu/game_title.png");
	app->audio->PlayFx(3, 0);
	e = world->create();
	e->assign<Position>(fPoint(0, -105));
	e->assign<Sprite>(t, 0.5f, 1);

	app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 148, 120, 32 }), 0);
	app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 201, 120, 32 }), 1);
	app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 256, 120, 32 }), 2);
	app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 311, 120, 32 }), 3);

	t = app->tex->Load("Assets/Textures/UI/MainMenu/start.png");
	e = world->create();
	e->assign<Position>(fPoint(0, 0));
	e->assign<Sprite>(t, 0.5f, 1);

	t = app->tex->Load("Assets/Textures/UI/MainMenu/continue.png");
	e = world->create();
	e->assign<Position>(fPoint(0, 0));
	e->assign<Sprite>(t, 0.5f, 1);

	t = app->tex->Load("Assets/Textures/UI/MainMenu/options.png");
	e = world->create();
	e->assign<Position>(fPoint(0, 0));
	e->assign<Sprite>(t, 0.5f, 1);

	t = app->tex->Load("Assets/Textures/UI/MainMenu/exit.png");
	e = world->create();
	e->assign<Position>(fPoint(0, 0));
	e->assign<Sprite>(t, 0.5f, 1);
}

void BattleScene::Load()
{
	Scene::Load();
	world->registerSystem(new SpriteSystem());

	SDL_Texture* t = app->tex->Load("Assets/Textures/Battle/battle_forest.png");
	ECS::Entity* e = world->create();
	app->render->camera.x = app->render->camera.y = 0;
	e->assign<Position>(fPoint(0, 0));
	e->assign<Sprite>(t, 0.5f, 1);

}
