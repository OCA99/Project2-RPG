#pragma once

#include "ECS.h"
#include "Components.h"

class MenuFadeSystem : public ECS::EntitySystem
{
public:
	MenuFadeSystem() {}

	virtual ~MenuFadeSystem() {}

	virtual void tick(ECS::World* world, float dt) override;

	int direction = -1;
	float alpha = 450.0f;

	bool hasSounded = false;
	bool hasFinished = false;
};