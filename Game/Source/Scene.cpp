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
#include "BattleManager.h"
#include "PartyManager.h"
#include "BattleMemberSystem.h"
#include "MenuFadeSystem.h"
#include "SceneTransitionSystem.h"

#include "BattleMemberFactory.h"

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
	world->registerSystem(new SceneTransitionSystem());

	ECS::Entity* e = world->create();

	std::string s = filename;
	e->assign<SceneFade>(250, 16);
	
	if (s == "Town.tmx")
	{
		app->audio->songToBeLoaded = "Audio/Music/Originals/town_main.wav";
		app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);
	}
	else if (s == "Tavern.tmx")
	{
		app->audio->songToBeLoaded = "Audio/Music/Originals/town_tavern.wav";
		app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);
	}
	else if (s == "Forest.tmx")
	{
		app->audio->songToBeLoaded = "Audio/Music/Originals/forest_main.wav";
		app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);
	}
	else if (s == "NPC House.tmx")
	{
		app->audio->songToBeLoaded = "Audio/Music/Originals/town_house.wav";
		app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);
	}
	else if (s == "Graveyard.tmx")
	{
		app->audio->songToBeLoaded = "Audio/Music/graveyard_main.ogg";
		app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);
	}
	else if (s == "Sewers.tmx")
	{
		app->audio->songToBeLoaded = "Audio/Music/town_blacksmith.ogg";
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
	world->registerSystem(new AnimatorSystem());

	ECS::Entity* e = world->create();

	e->assign<Position>(fPoint(150, 50));

	app->audio->LoadFx("Audio/Fx/logo.ogg");//1
	app->audio->LoadFx("Audio/Fx/action_interact.ogg");//2
	app->audio->LoadFx("Audio/Fx/title.ogg");//3
	app->audio->LoadFx("Audio/Fx/action_door.ogg");//4
	app->audio->LoadFx("Audio/Fx/player_footsteps_ground.ogg");//5
	app->audio->LoadFx("Audio/Fx/menu_interact.ogg");//6
	app->audio->LoadFx("Audio/Fx/player_footsteps_ground.ogg");//7
	app->audio->LoadFx("Audio/Fx/menu_travel.ogg");//8
	app->audio->LoadFx("Audio/Fx/all_attack.ogg");//9
	app->audio->LoadFx("Audio/Fx/all_heal.ogg");//10

	logoTexture = app->tex->Load("Textures/UI/MainPauseMenu/LogoAnim/logo_anim_tileset.png");

	e->assign<Sprite>(logoTexture, 1.0f);
	ECS::ComponentHandle<Animator> a = e->assign<Animator>();
	a->CreateAnimation("idle", SDL_Rect({ 0, 0, 1600, 1200 }), 5, 5, 3, false);
	a->SetAnimation("idle");

	e = world->create();
	e->assign<LogoFade>(70);
	app->audio->PlayFx(1, 0);

}

void MenuScene::Load()
{
	Scene::Load();

	world->registerSystem(new SpriteSystem());
	world->registerSystem(new MenuFadeSystem());

	app->audio->songToBeLoaded = "Audio/Music/Originals/forest_start.wav";
	app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);

	mainMenu = app->tex->Load("Textures/UI/MainPauseMenu/main_menu.png");

	ECS::Entity* e = world->create();
	e->assign<Position>(fPoint(0, 0));
	e->assign<Sprite>(mainMenu, 0.5f);

	gameTitle = app->tex->Load("Textures/UI/MainPauseMenu/game_title.png");
	//app->audio->PlayFx(3, 0);
	e = world->create();
	e->assign<Position>(fPoint(0, -105));
	e->assign<MenuFade>(70);
	e->assign<Sprite>(gameTitle, 0.5f, 1);

	app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 148, 120, 32 }), 0); //start
	app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 201, 120, 32 }), 1); //continue
	app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 256, 120, 32 }), 2); //options
	app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 262, 311, 120, 32 }), 3); //exit

	app->render->camera.x = app->render->camera.y = 0;
}

void BattleScene::Load()
{
	if(app->map->currentMapName == "Forest.tmx") app->audio->songToBeLoaded = "Audio/Music/Originals/forest_battle.wav";
	if(app->map->currentMapName == "Graveyard.tmx") app->audio->songToBeLoaded = "Audio/Music/Originals/graveyard_battle.wav";
	app->audio->PlayMusic(app->audio->songToBeLoaded, 0.f);

	Scene::Load();
	world->registerSystem(new SpriteSystem());
	world->registerSystem(new BattleMemberSystem());
	world->registerSystem(new AnimatorSystem());

	if (app->map->currentMapName == "Forest.tmx") background = app->tex->Load("Textures/Battle/battle_forest.png");
	if (app->map->currentMapName == "Graveyard.tmx") background = app->tex->Load("Textures/Battle/battle_cemetary.png");
	ECS::Entity* e = world->create();
	app->render->camera.x = app->render->camera.y = 0;
	e->assign<Position>(fPoint(0, 0));
	e->assign<Sprite>(background, 0.5f, 1);

	app->battle->StartBattle();
	for (int i = 0; i < app->party->allyParty->list.size(); i++)
	{
		BattleMemberFactory::Create(world, fPoint(40.0f, 50 + i * 80.5f), app->party->allyParty->list.at(i));
	}
	for (int i = 0; i < app->party->enemyParty->list.size(); i++)
	{
		BattleMemberFactory::Create(world, fPoint(480.0f, 20 + i * 80.5f), app->party->enemyParty->list.at(i));
	}
	
}
