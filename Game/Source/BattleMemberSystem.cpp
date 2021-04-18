#include "BattleMemberSystem.h"
#include "Components.h"
#include "App.h"
#include "BattleManager.h"
#include "PartyManager.h"

void BattleMemberSystem::tick(ECS::World* world, float dt)
{
	world->each<BattleMember>([&](ECS::Entity* entity, ECS::ComponentHandle<BattleMember> battleMember) {
		if(battleMember->member->state == Member::State::ATTACKING)
			entity->get<Animator>()->SetAnimation("attacking");
		if (battleMember->member->state == Member::State::HURT)
			entity->get<Animator>()->SetAnimation("hurt");
		if (battleMember->member->state == Member::State::DEAD)
			entity->get<Animator>()->SetAnimation("dead");
		if (battleMember->member->state == Member::State::IDLE)
			entity->get<Animator>()->SetAnimation("idle");

		ECS::ComponentHandle<Animator> a = entity->get<Animator>();

		if (a->currentAnimation->HasFinished() && a->currentAnimation != a->animations.at("dead"))
		{
			a->currentAnimation->Reset();
			battleMember->member->state = Member::State::IDLE;
		}

	});
}
