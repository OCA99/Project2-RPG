#pragma once

#include "ECS.h"
#include "Components.h"

class BattleMemberSystem : public ECS::EntitySystem
{
public:
	BattleMemberSystem() {}

	virtual ~BattleMemberSystem() {}

	virtual void tick(ECS::World* world, float dt) override;
};