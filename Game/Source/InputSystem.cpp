#include "InputSystem.h"
#include "ECS.h"

#include "App.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"

void InputSystem::tick(ECS::World* world, float dt)
{
	world->each<KeyboardMovement>([&](ECS::Entity* entity, ECS::ComponentHandle<KeyboardMovement> keyboardMovement) {
		ECS::ComponentHandle<Position> p = entity->get<Position>();

		fPoint total = fPoint(0.f, 0.f);

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			total += fPoint(1.f, 0.f);
		}

		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			total += fPoint(0.f, -1.f);
		}

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			total += fPoint(-1.f, 0.f);
		}

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			total += fPoint(0.f, 1.f);
		}

		p->Translate(total.Normalize() * keyboardMovement->speed * dt);
	});
}