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

		return player;
	}
};