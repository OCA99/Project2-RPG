#pragma once

#include "ECS.h"
#include "Components.h"

class CameraSystem : public ECS::EntitySystem
{
public:
	CameraSystem() {}

	virtual ~CameraSystem() {}

	virtual void tick(ECS::World* world, float dt) override;
};