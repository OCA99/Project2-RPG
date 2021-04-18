#include "SceneTransitionSystem.h"
#include "Render.h"
#include "Window.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Input.h"
#include "Audio.h"
#include "App.h"

#include <algorithm>

void SceneTransitionSystem::tick(ECS::World* world, float dt)
{
	world->each<SceneFade>([&](ECS::Entity* entity, ECS::ComponentHandle<SceneFade> sceneFade) {
		GamePad& pad = app->input->pads[0];
		SDL_Rect fullscreen;
		fullscreen.x = 0;
		fullscreen.y = 0;
		uint w, h;
		app->win->GetWindowSize(w, h);
		fullscreen.w = w;
		fullscreen.h = h;

		alpha -= sceneFade->speed * dt * direction;
		if (alpha < 0 && direction == 1)
		{
			alpha = 0;

			hasFinished = true;
		}

		if (alpha > 255 && direction == -1)
		{
			direction *= -1;
			alpha = 255;
		}

		if (alpha > 200 && !hasSounded)
		{
			//app->audio->PlayFx(3, 0);
			hasSounded = true;
		}
		app->render->DrawRectangle(fullscreen, 0, 0, 0, std::min(int(alpha), 255), true, false);
		});
}
