#pragma once

#include "Prefab.h"
#include "Components.h"
#include "App.h"
#include "Textures.h"
#include "Point.h"

static class PlayerFactory : public Prefab
{
public:
	static ECS::Entity* Create(ECS::World* world, fPoint position)
	{
		ECS::Entity* player = Create(world);
		player->get<Position>()->position = position;

		return player;
	}
protected:
	static ECS::Entity* Create(ECS::World* world)
	{
		ECS::Entity* player = world->create();
		player->assign<Position>();

		SDL_Texture* t = app->tex->Load("Assets/Textures/Characters/Characters.png");
		player->assign<Sprite>(t, SDL_Rect({ 0, 0, 48, 96 }));

		player->assign<Animator>();
		player->get<Animator>()->CreateAnimation("idle", SDL_Rect({ 16, 0, 16, 32 }));
		player->get<Animator>()->SetAnimation("idle");

		player->get<Animator>()->CreateAnimation("walk_down", SDL_Rect({ 0, 0, 48, 32 }), 1, 3, 8.f);
		player->get<Animator>()->CreateAnimation("walk_left", SDL_Rect({ 0, 32, 48, 32 }), 1, 3, 8.f);
		player->get<Animator>()->CreateAnimation("walk_right", SDL_Rect({ 0, 64, 48, 32 }), 1, 3, 8.f);
		player->get<Animator>()->CreateAnimation("walk_up", SDL_Rect({0, 96, 48, 32}), 1, 3, 8.f);

		player->assign<KeyboardMovement>(2.f);

		player->assign<Humanoid>();

		return player;
	}
};