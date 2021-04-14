#include "SpriteSystem.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "Log.h"

bool compareQueue(SpriteSystem::RenderQueue a, SpriteSystem::RenderQueue b) {
	return a.y < b.y;
}

void SpriteSystem::tick(ECS::World* world, float dt)
{
	world->each<Sprite>([&](ECS::Entity* entity, ECS::ComponentHandle<Sprite> sprite) {
		ECS::ComponentHandle<Position> p = entity->get<Position>();
		ECS::ComponentHandle<Animator> a = entity->get<Animator>();

		RenderQueue q;
		q.texture = sprite->sprite;
		q.x = p->position.x;
		q.y = p->position.y;

		if (a.isValid() && a->currentAnimation != nullptr)
		{
			q.section = &a->currentAnimation->GetCurrentFrame();
			q.scale = sprite->scale;
		}
		queue->push_back(q);
	});

	std::sort(queue->begin(), queue->end(), compareQueue);

	for (int i = 0; i < queue->size(); i++)
	{
		RenderQueue q = queue->at(i);
		app->render->DrawTexture(q.texture, q.x, q.y, q.section, q.scale);
	}

	queue->clear();
}

void SpriteSystem::unconfigure(ECS::World* world)
{
	world->each<Sprite>([&](ECS::Entity* entity, ECS::ComponentHandle<Sprite> sprite) {
		app->tex->UnLoad(sprite->sprite);
	});
}
