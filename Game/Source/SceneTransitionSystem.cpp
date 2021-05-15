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
		//if (app->volume > 100 && app->volumeUp == false)
		//{
		//	app->volume = 100;
		//	app->volume += speed * dt;

		//}
		//Mix_VolumeMusic(app->volume);


		if (alpha > 0 && direction == -1)
		{
			direction *= -1;
			alpha = 255;
		}

		int squareSize = fullscreen.w / sceneFade->squares;

		for (int i = 0; i < sceneFade->squares; i++) {
			for (int j = 0; j < sceneFade->squares; j++) {
				SDL_Rect rect;
				float prop = std::min(alpha, 255.0f) / 255.0f;
				if ((i - j) % 2 == 0)
					rect = SDL_Rect({ i * squareSize, j * squareSize, squareSize, int(std::round(squareSize * prop))});
				else
					rect = SDL_Rect({ i * squareSize, j * squareSize + int(std::round(squareSize * (1 - prop))), squareSize, int(std::round(squareSize * prop))});
				app->render->DrawRectangle(rect, 0, 0, 0, 255, true, false);
			}
		}

		if (alpha <= 10)
		{
			world->destroy(entity, true);
		}
		});
}
