#include "CollisionSystem.h"

#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Map.h"
#include "Window.h"

#include "EventHandler.h"

#include <vector>
#include <math.h>

void CollisionSystem::tick(ECS::World* world, float dt)
{
	/*world->each<NPCCollider>([&](ECS::Entity* npcEntity, ECS::ComponentHandle<NPCCollider> npcCollider) {
		fPoint npcPosition = npcEntity->get<Position>()->position;

		SDL_Rect rect = SDL_Rect({ int(npcPosition.x + npcCollider->rect.x), int(npcPosition.y + npcCollider->rect.y), npcCollider->rect.w, npcCollider->rect.h });

		//app->render->DrawRectangle(rect, 255, 0, 0, 128);
	});*/

	world->each<MapCollider>([&](ECS::Entity* entity, ECS::ComponentHandle<MapCollider> collider) {
		fPoint position = entity->get<Position>()->position;
		SDL_Rect offsetRect = GetOffsetRect(collider, position);

		std::vector<SDL_Rect>* intersectors = app->map->NavigationIntersection(offsetRect);

		world->each<NPCCollider>([&](ECS::Entity* npcEntity, ECS::ComponentHandle<NPCCollider> npcCollider) {
			fPoint npcPosition = npcEntity->get<Position>()->position;

			intersectors->push_back(SDL_Rect({ int(npcPosition.x + npcCollider->rect.x), int(npcPosition.y + npcCollider->rect.y), npcCollider->rect.w, npcCollider->rect.h }));

			world->each<DialogTrigger>([&](ECS::Entity* entity, ECS::ComponentHandle<DialogTrigger> trigger) {
				SDL_Rect offsetTrigger = SDL_Rect({ int(trigger->rect.x + npcPosition.x), int(trigger->rect.y + npcPosition.y), trigger->rect.w, trigger->rect.h });

				if (Intersects(offsetRect, offsetTrigger) && !trigger->started)
				{
					trigger->started = true;
					app->dialog->StartDialog("TEST");
				}
			});
		});

		fPoint deltaPosition = entity->get<Position>()->deltaMovement;

		if (intersectors->size() != 0) // There is a collision with map navigation
		{
			for (const SDL_Rect& inter : *intersectors)
			{
				SDL_Rect tmpRect = GetOffsetRect(collider, entity->get<Position>()->position);

				if (!Intersects(tmpRect, inter))
					continue;

				tmpRect.x -= (deltaPosition.x > 0) ? deltaPosition.x : std::floor(deltaPosition.x);
				if (!Intersects(tmpRect, inter))
				{
					entity->get<Position>()->position.x -= deltaPosition.x;
					continue;
				}

				tmpRect.x += (deltaPosition.x > 0) ? deltaPosition.x : std::floor(deltaPosition.x);
				tmpRect.y -= (deltaPosition.y > 0) ? deltaPosition.y : std::floor(deltaPosition.y);
				if (!Intersects(tmpRect, inter))
				{
					entity->get<Position>()->position.y -= deltaPosition.y;
					continue;
				}

				tmpRect.x -= (deltaPosition.x > 0) ? deltaPosition.x : std::floor(deltaPosition.x);
				if (!Intersects(tmpRect, inter))
				{
					entity->get<Position>()->position.x -= deltaPosition.x;
					entity->get<Position>()->position.y -= deltaPosition.y;
					continue;
				}

				tmpRect.x += (deltaPosition.x > 0) ? deltaPosition.x : std::floor(deltaPosition.x);
				tmpRect.y += (deltaPosition.y > 0) ? deltaPosition.y : std::floor(deltaPosition.y);
			}
		}

		delete intersectors;
	});

	world->each<EventCollider>([&](ECS::Entity* entity, ECS::ComponentHandle<EventCollider> collider) {
		fPoint position = entity->get<Position>()->position;
		SDL_Rect offsetRect = GetOffsetRect(collider, position);

		std::pair<int, int> result;
		bool found = app->map->EventIntersection(offsetRect, result);

		if (found)
		{
			MapEvent* e = app->map->GetEvent(result.first, result.second);
			
			if (e == nullptr)
				return;

			EventHandler::FireEvent(e);
		}
	});
}