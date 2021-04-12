#pragma once

#include "ECS.h"
#include "Components.h"

class CollisionSystem : public ECS::EntitySystem
{
public:
	CollisionSystem() {}

	virtual ~CollisionSystem() {}

	virtual void tick(ECS::World* world, float dt) override;

	bool Intersects(SDL_Rect a, SDL_Rect b)
	{
		int leftX = std::max(a.x, b.x);
		int rightX = std::min(a.x + a.w, b.x + b.w);
		int topY = std::max(a.y, b.y);
		int bottomY = std::min(a.y + a.h, b.y + b.h);

		if (leftX < rightX && topY < bottomY)
			return true;
		else
			return false;
	}

	SDL_Rect GetOffsetRect(ECS::ComponentHandle<MapCollider> collider, fPoint position)
	{
		return SDL_Rect({ collider->rect.x + int(std::round(position.x)), collider->rect.y + int(std::round(position.y)), collider->rect.w, collider->rect.h });
	}

	SDL_Rect GetOffsetRect(ECS::ComponentHandle<EventCollider> collider, fPoint position)
	{
		return SDL_Rect({ collider->rect.x + int(std::round(position.x)), collider->rect.y + int(std::round(position.y)), collider->rect.w, collider->rect.h });
	}
};