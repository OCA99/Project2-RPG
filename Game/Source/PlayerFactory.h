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

		SDL_Texture* t = app->tex->Load("Assets/Textures/Characters/toisto_good_sprites.png");
		player->assign<Sprite>(t, 0.8f);

		player->assign<Animator>();
		player->get<Animator>()->CreateAnimation("idle", SDL_Rect({ 25, 0, 25, 26 }));
		player->get<Animator>()->SetAnimation("idle");
		player->get<Animator>()->CreateAnimation("walk_down", SDL_Rect({ 0, 0, 75, 26 }), 1, 3, 8.f, true, true);
		player->get<Animator>()->CreateAnimation("walk_left", SDL_Rect({ 0, 26, 75, 26 }), 1, 3, 8.f, true, true);
		player->get<Animator>()->CreateAnimation("walk_right", SDL_Rect({ 0, 52, 75, 26 }), 1, 3, 8.f, true, true);
		player->get<Animator>()->CreateAnimation("walk_up", SDL_Rect({0, 78, 75, 26}), 1, 3, 8.f, true, true);

		player->assign<KeyboardMovement>(70.f);
		player->assign<Humanoid>();
		player->assign<CameraFollower>();

		SDL_Rect colliderRect;
		colliderRect.x = 4 * 0.8f;
		colliderRect.y = 18 * 0.8f;
		colliderRect.w = 17 * 0.8f;
		colliderRect.h = 8 * 0.8f;

		player->assign<MapCollider>(colliderRect);
		player->assign<EventCollider>(colliderRect);

		return player;
	}
};