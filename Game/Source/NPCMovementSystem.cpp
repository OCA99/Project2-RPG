#include "NPCMovementSystem.h"

void NPCMovementSystem::tick(ECS::World* world, float dt)
{
	world->each<NPCMover>([&](ECS::Entity* entity, ECS::ComponentHandle<NPCMover> npcMover) {
		ECS::ComponentHandle<Position> p = entity->get<Position>();

		p->Translate(fPoint(npcMover->speed, 0) * dt);
	});

	world->each<NPCMoverAuto>([&](ECS::Entity* entity, ECS::ComponentHandle<NPCMoverAuto> npcMoverAuto) {
		ECS::ComponentHandle<Position> p = entity->get<Position>();
		if (npcMoverAuto->speed != 0)
		{
			if (npcMoverAuto->x <= 0 && npcMoverAuto->y <= 0)
			{
				npcMoverAuto->speed = 0;
			}
			else if (npcMoverAuto->x > 0)
			{
				p->Translate(fPoint(npcMoverAuto->speed, 0) * dt);
				npcMoverAuto->x--;
			}
			else if (npcMoverAuto->y > 0)
			{
				p->Translate(fPoint(0, npcMoverAuto->speed) * dt);
				npcMoverAuto->y--;
			}

		}
		else { p->Translate(fPoint(0, 0) * dt); }

		//p->Translate(fPoint(npcMover->speed, -1) * dt);

		});
}
