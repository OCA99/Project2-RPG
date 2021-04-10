#pragma once

#include "ECS.h"
#include "Components.h"

class InputSystem : public ECS::EntitySystem
{
public:
	InputSystem() {}

	virtual ~InputSystem() {}

	virtual void tick(ECS::World* world, float dt) override;
};