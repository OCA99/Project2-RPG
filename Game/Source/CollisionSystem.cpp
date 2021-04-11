#include "CollisionSystem.h"

#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Map.h"
#include "Window.h"

#include <vector>
#include <math.h>

void CollisionSystem::tick(ECS::World* world, float dt)
{
	world->each<MapCollider>([&](ECS::Entity* entity, ECS::ComponentHandle<MapCollider> collider) {
		fPoint position = entity->get<Position>()->position;
		SDL_Rect offsetRect = GetOffsetRect(collider, position);

		std::vector<SDL_Rect>* intersectors = app->map->NavigationIntersection(offsetRect);

		//app->render->DrawRectangle(offsetRect, 255, 0, 0, 128);

		fPoint deltaPosition = entity->get<Position>()->deltaMovement;

		if (intersectors->size() != 0) // There is a collision with map navigation
		{
			for (const SDL_Rect& inter : *intersectors)
			{
				SDL_Rect tmpRect = GetOffsetRect(collider, entity->get<Position>()->position);

				if (!Intersects(tmpRect, inter))
					continue;

				tmpRect.x -= deltaPosition.x;
				if (!Intersects(tmpRect, inter))
				{
					entity->get<Position>()->position.x -= deltaPosition.x;
					continue;
				}

				tmpRect.x += deltaPosition.x;
				tmpRect.y -= deltaPosition.y;
				if (!Intersects(tmpRect, inter))
				{
					entity->get<Position>()->position.y -= deltaPosition.y;
					continue;
				}

				tmpRect.x -= deltaPosition.x;
				if (!Intersects(tmpRect, inter))
				{
					entity->get<Position>()->position.x -= deltaPosition.x;
					entity->get<Position>()->position.y -= deltaPosition.y;
					continue;
				}

				tmpRect.x += deltaPosition.x;
				tmpRect.y += deltaPosition.y;
			}
		}
	});
}