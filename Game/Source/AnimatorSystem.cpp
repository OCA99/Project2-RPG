#include "AnimatorSystem.h"

#include "Log.h"
#include "Animation.h"

void AnimatorSystem::tick(ECS::World* world, float dt)
{
	world->each<Animator>([&](ECS::Entity* entity, ECS::ComponentHandle<Animator> animator) {
		if (animator->currentAnimation != nullptr)
			animator->currentAnimation->Update(dt);
	});
}