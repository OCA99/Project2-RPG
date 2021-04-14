#pragma once

#include "ECS.h"
#include "Components.h"

#include <vector>
#include <algorithm>

class SpriteSystem : public ECS::EntitySystem
{
public:
	SpriteSystem() {
		queue = new std::vector<RenderQueue>();
	}

	virtual ~SpriteSystem() {}

	virtual void tick(ECS::World* world, float dt) override;

	virtual void unconfigure(ECS::World* world) override;

	struct RenderQueue {
		SDL_Texture* texture;
		int x;
		int y;
		const SDL_Rect* section = nullptr;
		float scale = 1.0f;
	};

	std::vector<RenderQueue>* queue;

private:

};