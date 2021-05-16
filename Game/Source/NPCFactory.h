#pragma once

#include "Prefab.h"
#include "Components.h"
#include "App.h"
#include "Textures.h"
#include "Point.h"
#include "QuestManager.h"

static class NPCFactory : public Prefab
{
public:
	enum class Type {
		TAVERN,
		REAPER,
		CUSTOMER,
		THYMA
	};


	static ECS::Entity* Create(ECS::World* world, fPoint pos, Type type, float radius = 14)
	{
		ECS::Entity* npc = Create(world, type, radius);
		pos.y -= 14;
		npc->get<Position>()->position = pos;
		npc->get<QuestList>()->position = pos;

		return npc;
	}
protected:
	static ECS::Entity* Create(ECS::World* world, Type type, float radius)
	{
		ECS::Entity* npc = world->create();
		npc->assign<Position>();
		npc->assign<QuestList>();

		if (type == Type::TAVERN)
		{
			SString name = "tlady";
			SDL_Texture* t = app->tex->Load("Textures/NPC/tavern_lady_sprites.png");
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


			npc->get<QuestList>()->SetReceiver(name);
			npc->get<QuestList>()->LoadActive(&app->quests->questsActive);
			npc->get<QuestList>()->LoadInactive(&app->quests->questsInactive);

		}
		else if (type == Type::REAPER)
		{

			SString name = "reaper";

			SDL_Texture* t = app->tex->Load("Textures/NPC/demon_reaper_sprites.png");
			npc->assign<Sprite>(t, 0.8f);

			npc->assign<Animator>();
			npc->get<Animator>()->CreateAnimation("idle", SDL_Rect({ 27, 0, 27, 26 }));
			npc->get<Animator>()->SetAnimation("idle");
			npc->get<Animator>()->CreateAnimation("walk_down", SDL_Rect({ 0, 0, 81, 26 }), 1, 3, 8.f, true, true);
			npc->get<Animator>()->CreateAnimation("walk_left", SDL_Rect({ 0, 26, 81, 26 }), 1, 3, 8.f, true, true);
			npc->get<Animator>()->CreateAnimation("walk_right", SDL_Rect({ 0, 52, 81, 26 }), 1, 3, 8.f, true, true);
			npc->get<Animator>()->CreateAnimation("walk_up", SDL_Rect({ 0, 78, 81, 26 }), 1, 3, 8.f, true, true);

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

			npc->assign<DialogTrigger>(collider, "reaper.xml", "REAPER");

			npc->get<QuestList>()->SetReceiver(name);
			npc->get<QuestList>()->LoadActive(&app->quests->questsActive);
			npc->get<QuestList>()->LoadInactive(&app->quests->questsInactive);
		}
		else if (type == Type::CUSTOMER)
		{

			SString name = "customer";

			SDL_Texture* t = app->tex->Load("Textures/NPC/blacksmith_sprites.png");
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

			npc->assign<DialogTrigger>(collider, "customer.xml", "CUSTOMER");

			npc->get<QuestList>()->SetReceiver(name);
			npc->get<QuestList>()->LoadActive(&app->quests->questsActive);
			npc->get<QuestList>()->LoadInactive(&app->quests->questsInactive);
		}
		else if (type == Type::THYMA)
		{

			SString name = "thyma";

			SDL_Texture* t = app->tex->Load("Textures/Characters/thyma_good_sprites.png");
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

			npc->assign<DialogTrigger>(collider, "thyma.xml", "THYMA");

			npc->get<QuestList>()->SetReceiver(name);
			npc->get<QuestList>()->LoadActive(&app->quests->questsActive);
			npc->get<QuestList>()->LoadInactive(&app->quests->questsInactive);
		}

		return npc;
	}
};