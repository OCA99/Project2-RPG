#pragma once

#include "Prefab.h"
#include "Components.h"
#include "App.h"
#include "Textures.h"
#include "Point.h"
#include "PartyManager.h"

static class BattleMemberFactory : public Prefab
{
public:
	static ECS::Entity* Create(ECS::World* world, fPoint position, Member* member)
	{
		ECS::Entity* nMember = Create(world, member);
		nMember->get<Position>()->position = position;
		nMember->assign<BattleMember>(member);
		return nMember;
	}


protected:
	static ECS::Entity* Create(ECS::World* world, Member* member)
	{
		ECS::Entity* nMember = world->create();
		nMember->assign<Position>();

		if (member->type == Type::PLAYER1)
		{
			SDL_Texture** t = app->tex->Load("Textures/Battle/toisto_battle_sprites.png");
			nMember->assign<Sprite>(t, 0.3f,2);

			nMember->assign<Animator>();
			nMember->get<Animator>()->CreateAnimation("idle", SDL_Rect({ 530, 0, 530, 245 }),1,2);
			nMember->get<Animator>()->SetAnimation("idle");
			nMember->get<Animator>()->CreateAnimation("attacking", SDL_Rect({ 0, 0, 265, 245 }),1,1,1.0f,false);
			nMember->get<Animator>()->CreateAnimation("hurt", SDL_Rect({ 0, 245, 265, 245 }), 1,1,1.0f,false);
			nMember->get<Animator>()->CreateAnimation("dead", SDL_Rect({ 0, 245, 795, 245 }), 1, 3, 1.0f,false);

		}
		else if (member->type == Type::PLAYER2)
		{
			SDL_Texture** t = app->tex->Load("Textures/Battle/thyma_battle_sprites.png");
			nMember->assign<Sprite>(t, 0.3f,2);

			nMember->assign<Animator>();
			nMember->get<Animator>()->CreateAnimation("idle", SDL_Rect({ 530, 0, 530, 245 }), 1, 2);
			nMember->get<Animator>()->SetAnimation("idle");
			nMember->get<Animator>()->CreateAnimation("attacking", SDL_Rect({ 0, 0, 265, 245 }), 1, 1, 1.0f, false);
			nMember->get<Animator>()->CreateAnimation("hurt", SDL_Rect({ 0, 245, 265, 245 }), 1, 1, 1.0f, false);
			nMember->get<Animator>()->CreateAnimation("dead", SDL_Rect({ 0, 245, 795, 245 }), 1, 3, 1.0f, false);
		}
		else if (member->type == Type::SKELETON)
		{
			SDL_Texture** t = app->tex->Load("Textures/Battle/skeleton_battle_sprites.png");
			nMember->assign<Sprite>(t, 0.3f,2);

			nMember->assign<Animator>();
			nMember->get<Animator>()->CreateAnimation("idle", SDL_Rect({ 0, 0, 530, 245 }), 1, 2);
			nMember->get<Animator>()->SetAnimation("idle");
			nMember->get<Animator>()->CreateAnimation("attacking", SDL_Rect({ 795, 0, 265, 245 }), 1, 1, 1.0f, false);
			nMember->get<Animator>()->CreateAnimation("hurt", SDL_Rect({ 0, 245, 265, 245 }), 1, 1, 1.0f, false);
			nMember->get<Animator>()->CreateAnimation("dead", SDL_Rect({ 0, 245, 530, 245 }), 1, 2, 1.0f, false);
		}
		else if (member->type == Type::SHROOM1)
		{
			SDL_Texture** t = app->tex->Load("Textures/Battle/shroom_battle_sprites.png");
			nMember->assign<Sprite>(t, 0.3f,2);

			nMember->assign<Animator>();
			nMember->get<Animator>()->CreateAnimation("idle", SDL_Rect({ 0, 0, 490, 210 }), 1, 2);
			nMember->get<Animator>()->SetAnimation("idle");
			nMember->get<Animator>()->CreateAnimation("attacking", SDL_Rect({ 490, 0, 245, 210 }), 1, 1, 1.0f, false);
			nMember->get<Animator>()->CreateAnimation("hurt", SDL_Rect({ 0, 210, 245, 210 }), 1, 1, 1.0f, false);
			nMember->get<Animator>()->CreateAnimation("dead", SDL_Rect({ 0, 210, 490, 210 }), 1, 2, 1.0f, false);
		}
		else if (member->type == Type::SHROOM2)
		{
			SDL_Texture** t = app->tex->Load("Textures/Battle/shroom2_battle_sprites.png");
			nMember->assign<Sprite>(t, 0.3f,2);

			nMember->assign<Animator>();
			nMember->get<Animator>()->CreateAnimation("idle", SDL_Rect({ 0, 0, 490, 210 }), 1, 2);
			nMember->get<Animator>()->SetAnimation("idle");
			nMember->get<Animator>()->CreateAnimation("attacking", SDL_Rect({ 490, 0, 490, 210 }), 1, 2, 1.0f, false);
			nMember->get<Animator>()->CreateAnimation("hurt", SDL_Rect({ 0, 210, 245, 210 }), 1, 1, 1.0f, false);
			nMember->get<Animator>()->CreateAnimation("dead", SDL_Rect({ 0, 210, 490, 210 }), 1, 2, 1.0f, false);
		}

		return nMember;
	}
};