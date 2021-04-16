#pragma once

#include "ECS.h"
#include "Components.h"

class LogoFadeSystem : public ECS::EntitySystem
{
public:
	LogoFadeSystem() {}

	virtual ~LogoFadeSystem() {}

	virtual void tick(ECS::World* world, float dt) override;

	int direction = -1;
	float alpha = 450.0f;

	bool hasSounded = false;
};