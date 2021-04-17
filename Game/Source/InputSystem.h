#pragma once

#include "ECS.h"
#include "Components.h"
#include "App.h"
#include "Input.h"

class InputSystem : public ECS::EntitySystem
{
public:
	InputSystem() {}

	virtual ~InputSystem() {}

	virtual void tick(ECS::World* world, float dt) override;

	GamePad& pad = app->input->pads[0];
};