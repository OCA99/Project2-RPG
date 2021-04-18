#include "MenuFadeSystem.h"
#include "Render.h"
#include "Window.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Input.h"
#include "Audio.h"
#include "App.h"

#include <algorithm>

void MenuFadeSystem::tick(ECS::World* world, float dt)
{
	world->each<MenuFade>([&](ECS::Entity* entity, ECS::ComponentHandle<MenuFade> MenuFade) {
		GamePad& pad = app->input->pads[0];
		SDL_Rect fullscreen;
		fullscreen.x = 0;
		fullscreen.y = 0;
		uint w, h;
		app->win->GetWindowSize(w, h);
		fullscreen.w = w;
		fullscreen.h = h;

		
		/*if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.a == true && hasFinished == true) {
			direction = 1;
			alpha = 255;
		}*/

		alpha -= MenuFade->speed * dt * direction;
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
