#include "NPCMovementSystem.h"

void NPCMovementSystem::tick(ECS::World* world, float dt)
{
	world->each<NPCMover>([&](ECS::Entity* entity, ECS::ComponentHandle<NPCMover> npcMover) {
		ECS::ComponentHandle<Position> p = entity->get<Position>();

		p->Translate(fPoint(npcMover->speed, 0) * dt);
	});
}
