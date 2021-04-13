#include "SpriteSystem.h"
#include "App.h"
#include "Render.h"

#include "Log.h"

void SpriteSystem::tick(ECS::World* world, float dt)
{
	world->each<Sprite>([&](ECS::Entity* entity, ECS::ComponentHandle<Sprite> sprite) {
		ECS::ComponentHandle<Position> p = entity->get<Position>();
		ECS::ComponentHandle<Animator> a = entity->get<Animator>();

		if (a.isValid())
			app->render->DrawTexture(sprite->sprite, p->position.x, p->position.y, &a->currentAnimation->GetCurrentFrame(), sprite->scale);
		else
			app->render->DrawTexture(sprite->sprite, p->position.x, p->position.y);
	});
}