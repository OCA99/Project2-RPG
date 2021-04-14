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
	world->each<MapCollider>([&](ECS::Entity* entity, ECS::ComponentHandle<MapCollider> collider) {
		fPoint position = entity->get<Position>()->position;
		SDL_Rect offsetRect = GetOffsetRect(collider, position);

		std::vector<SDL_Rect>* intersectors = app->map->NavigationIntersection(offsetRect);

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