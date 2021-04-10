#include "PositionSystem.h"

#include "Log.h"
#include "Animation.h"

void PositionSystem::tick(ECS::World* world, float dt)
{
	world->each<Position>([&](ECS::Entity* entity, ECS::ComponentHandle<Position> position) {
		position->position += position->deltaMovement;
	});
}