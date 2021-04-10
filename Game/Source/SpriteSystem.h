#pragma once

#include "ECS.h"
#include "Components.h"

class SpriteSystem : public ECS::EntitySystem
{
public:
	SpriteSystem() {}

	virtual ~SpriteSystem() {}

	virtual void tick(ECS::World* world, float dt) override;
};