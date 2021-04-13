#pragma once

#include "Prefab.h"
#include "Components.h"
#include "App.h"
#include "Textures.h"
#include "Point.h"

static class NPCFactory : public Prefab
{
public:
	static ECS::Entity* Create(ECS::World* world, fPoint position)
	{
		ECS::Entity* npc = Create(world);
		npc->get<Position>()->position = position;

		return npc;
	}
protected:
	static ECS::Entity* Create(ECS::World* world)
	{
		ECS::Entity* npc = world->create();
		npc->assign<Position>();

		SDL_Texture* t = app->tex->Load("Assets/Textures/Characters/Characters.png");
		npc->assign<Sprite>(t, 0.8f);

		npc->assign<Animator>();
		npc->get<Animator>()->CreateAnimation("idle", SDL_Rect({ 208, 0, 16, 32 }));
		npc->get<Animator>()->SetAnimation("idle");
		npc->get<Animator>()->CreateAnimation("walk_down", SDL_Rect({ 192, 0, 48, 32 }), 1, 3, 8.f, true, true);
		npc->get<Animator>()->CreateAnimation("walk_left", SDL_Rect({ 192, 32, 48, 32 }), 1, 3, 8.f, true, true);
		npc->get<Animator>()->CreateAnimation("walk_right", SDL_Rect({ 192, 64, 48, 32 }), 1, 3, 8.f, true, true);
		npc->get<Animator>()->CreateAnimation("walk_up", SDL_Rect({ 192, 96, 48, 32 }), 1, 3, 8.f, true, true);

		npc->assign<NPCMover>(30.f);
		npc->assign<Humanoid>();

		return npc;
	}
};