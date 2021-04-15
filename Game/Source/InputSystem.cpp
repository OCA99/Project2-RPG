#include "InputSystem.h"
#include "ECS.h"
#include "DialogSytem.h"

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

		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			keyboardMovement->speed = 200.0f;
		}

		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			keyboardMovement->speed = 70.0f;
		}

		if (app->dialog->currentDialog == nullptr)
			p->Translate(total.Normalize() * keyboardMovement->speed * dt);
		else
			p->Translate(fPoint(0, 0));
	});
}