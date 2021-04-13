#pragma once

#include "ECS.h"
#include "Components.h"

class NPCMovementSystem : public ECS::EntitySystem
{
public:
	NPCMovementSystem() {}

	virtual ~NPCMovementSystem() {}

	virtual void tick(ECS::World* world, float dt) override;
};