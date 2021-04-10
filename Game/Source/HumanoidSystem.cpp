#include "HumanoidSystem.h"
#include "ECS.h"
#include "Log.h"

#include <math.h>

void HumanoidSystem::tick(ECS::World* world, float dt)
{
	world->each<Humanoid>([&](ECS::Entity* entity, ECS::ComponentHandle<Humanoid> humanoid) {
		ECS::ComponentHandle<Position> p = entity->get<Position>();

		if (p->deltaMovement.Length() == 0)
		{
			entity->get<Animator>()->SetAnimation("idle");
			return;
		}
		
		float direction = std::atan2(p->deltaMovement.y, p->deltaMovement.x);
		
		direction = (direction >= 0) ? direction : 2 * M_PI + direction;

		if (direction >= M_PI / 4.f && direction < M_PI / 4.f * 3.f)
			entity->get<Animator>()->SetAnimation("walk_down");
		else if (direction >= M_PI / 4.f * 3.f && direction < M_PI / 4.f * 5.f)
			entity->get<Animator>()->SetAnimation("walk_left");
		else if (direction >= M_PI / 4.f * 5.f && direction < M_PI / 4.f * 7.f)
			entity->get<Animator>()->SetAnimation("walk_up");
		else
			entity->get<Animator>()->SetAnimation("walk_right");
	});
}