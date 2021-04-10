#include "SpriteRenderer.h"
#include "App.h"
#include "Render.h"

#include "Log.h"

void SpriteRenderer::tick(ECS::World* world, float dt)
{
	world->each<Sprite>([&](ECS::Entity* entity, ECS::ComponentHandle<Sprite> sprite) {
		ECS::ComponentHandle<Position> p = entity->get<Position>();

		//LOG("%f, %f\n", p->position.x, p->position.y);

		app->render->DrawTexture(sprite->sprite, p->position.x, p->position.y, &sprite->section);
	});
}