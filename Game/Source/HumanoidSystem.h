#pragma once

#include "ECS.h"
#include "Components.h"

class HumanoidSystem : public ECS::EntitySystem
{
public:
	HumanoidSystem() {}

	virtual ~HumanoidSystem() {}

	virtual void tick(ECS::World* world, float dt) override;
};