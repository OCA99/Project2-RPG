#include "GuiSlider.h"
#include "Window.h"
#include <stdio.h>      /* printf */
#include <math.h>       /* round, floor, ceil, trunc */

#include "Log.h"


GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	slider = { bounds.x, bounds.y, 30, 30 };

}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Start()
{

	return true;
}

bool GuiSlider::Update(Input* input, float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;
			// TODO.
			unit = bounds.w / 100;
			value = (mouseX - bounds.x) / unit;
			value = round(value);

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				for (int i = 1; i <= 100; i++)
				{
					if (i == value)
					{
						slider.x = ((i * unit) + bounds.x) - unit;
					}
				}
			}

			// If mouse button pressed -> Generate event!
			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}

		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiSlider::Draw(Render* render)
{
	// Draw the right button depending on state
	int scale = app->win->GetScale();
	SDL_Rect drawBounds = SDL_Rect({ bounds.x * scale, bounds.y * scale, bounds.w * scale, bounds.h * scale });

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(drawBounds, 255, 0, 0, 128, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(drawBounds, 0, 255, 0, 128, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(drawBounds, 0, 0, 255, 128, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(drawBounds, 255, 255, 0, 128, true, false);
		break;
	case GuiControlState::SELECTED:
		render->DrawRectangle(drawBounds, 0, 255, 255, 128, true, false);
		break;
	}

	return false;
}
