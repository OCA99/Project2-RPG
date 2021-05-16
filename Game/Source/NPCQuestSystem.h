#pragma once

#include "ECS.h"
#include "Components.h"
#include "QuestManager.h"
#include "Textures.h"

class NPCQuestSystem : public ECS::EntitySystem
{
public:
	NPCQuestSystem() {}

	virtual ~NPCQuestSystem() {}

	virtual void tick(ECS::World* world, float dt) override;


	void LoadTex();

	SDL_Texture** exclamation;

	bool loaded = false;

};

