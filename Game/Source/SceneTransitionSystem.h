#pragma once

#include "ECS.h"
#include "Components.h"

class SceneTransitionSystem : public ECS::EntitySystem
{
public:
	SceneTransitionSystem() {}

	virtual ~SceneTransitionSystem() {}

	virtual void tick(ECS::World* world, float dt) override;

	int direction = -1;
	float alpha = 450.0f;

	bool hasSounded = false;
	bool hasFinished = false;
};