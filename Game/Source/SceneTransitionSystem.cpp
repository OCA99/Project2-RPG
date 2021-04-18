#include "SceneTransitionSystem.h"
#include "Render.h"
#include "Window.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Input.h"
#include "Audio.h"
#include "App.h"
#include "SDL_mixer/include/SDL_mixer.h"



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

		if (hasFinished == false)
		{
			app->volume = 0;
			hasFinished = true;
		}

		alpha -= sceneFade->speed * dt * direction;
		app->volume += speed * dt;
		if (app->volume > 100) app->volume = 100;
		Mix_VolumeMusic(app->volume);


		if (alpha > 0 && direction == -1)
		{
			direction *= -1;
			alpha = 255;
		}
		app->render->DrawRectangle(fullscreen, 0, 0, 0, std::min(int(alpha), 255), true, false);

		if (alpha <= 0)
		{
			world->destroy(entity, true);
		}
		});
}
