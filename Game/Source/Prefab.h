#pragma once

#include "ECS.h"

static class Prefab
{
protected:
	virtual ECS::Entity* Create(ECS::World* world) = 0;
};