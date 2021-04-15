#pragma once

#include "Prefab.h"
#include "Components.h"
#include "App.h"
#include "Textures.h"
#include "Point.h"

static class NPCFactory : public Prefab
{
public:
	enum class Type {
		TAVERN
	};

	static ECS::Entity* Create(ECS::World* world, fPoint position, Type type, float radius = 14)
	{
		ECS::Entity* npc = Create(world, type, radius);
		position.y -= 14;
		npc->get<Position>()->position = position;

		return npc;
	}
protected:
	static ECS::Entity* Create(ECS::World* world, Type type, float radius)
	{
		ECS::Entity* npc = world->create();
		npc->assign<Position>();

		if (type == Type::TAVERN)
		{
			SDL_Texture* t = app->tex->Load("Assets/Textures/NPC/tavern_lady_sprites.png");
			npc->assign<Sprite>(t, 0.8f);

			npc->assign<Animator>();
			npc->get<Animator>()->CreateAnimation("idle", SDL_Rect({ 25, 0, 25, 26 }));
			npc->get<Animator>()->SetAnimation("idle");
			npc->get<Animator>()->CreateAnimation("walk_down", SDL_Rect({ 0, 0, 75, 26 }), 1, 3, 8.f, true, true);
			npc->get<Animator>()->CreateAnimation("walk_left", SDL_Rect({ 0, 26, 75, 26 }), 1, 3, 8.f, true, true);
			npc->get<Animator>()->CreateAnimation("walk_right", SDL_Rect({ 0, 52, 75, 26 }), 1, 3, 8.f, true, true);
			npc->get<Animator>()->CreateAnimation("walk_up", SDL_Rect({ 0, 78, 75, 26 }), 1, 3, 8.f, true, true);

			SDL_Rect collider;
			collider.x = 4 * 0.8f;
			collider.y = 18 * 0.8f;
			collider.w = 17 * 0.8f;
			collider.h = 8 * 0.8f;

			npc->assign<NPCCollider>(collider);
			npc->assign<Humanoid>();

			collider.x = -radius;
			collider.y = -radius;
			collider.w += radius * 2;
			collider.h += radius * 3;

			npc->assign<DialogTrigger>(collider, "tavern.xml", "TEST");
		}

		return npc;
	}
};