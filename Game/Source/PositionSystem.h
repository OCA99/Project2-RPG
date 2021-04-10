#pragma once

#include "ECS.h"
#include "Components.h"

class PositionSystem : public ECS::EntitySystem
{
public:
	PositionSystem() {}

	virtual ~PositionSystem() {}

	virtual void tick(ECS::World* world, float dt) override;
};