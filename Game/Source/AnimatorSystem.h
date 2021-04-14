#pragma once

#include "ECS.h"
#include "Components.h"

class AnimatorSystem : public ECS::EntitySystem
{
public:
	AnimatorSystem() {}

	virtual ~AnimatorSystem() {}

	virtual void tick(ECS::World* world, float dt) override;

	virtual void unconfigure(ECS::World* world) override;
};