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

void AnimatorSystem::unconfigure(ECS::World* world)
{
	/*world->each<Animator>([&](ECS::Entity* entity, ECS::ComponentHandle<Animator> animator) {
		std::map<std::string, Animation*>::iterator i = animator->animations.begin();
		for (i; i != animator->animations.end(); i++)
		{
			delete i->second;
		}

		animator->currentAnimation = nullptr;
		animator->animations.clear();
	});*/
}
