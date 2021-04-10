#pragma once

#include "ECS.h"
#include "Components.h"

class SpriteRenderer : public ECS::EntitySystem
{
public:
	SpriteRenderer() {}

	virtual ~SpriteRenderer() {}

	virtual void tick(ECS::World* world, float dt) override;
};