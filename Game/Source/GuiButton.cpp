#include "GuiButton.h"
#include "Window.h"
#include "App.h"


GuiButton::GuiButton(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt)
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

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				return NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;
	}
	return true;
}

bool GuiButton::Draw(Render* render)
{
	int scale = app->win->GetScale();

	SDL_Rect drawBounds = SDL_Rect({ bounds.x * scale, bounds.y * scale, bounds.w * scale, bounds.h * scale });
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED: 
		render->DrawRectangle(drawBounds, color.r, color.g, color.b, color.a - 200, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(drawBounds, color.r, color.g, color.b, color.a + 100, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(drawBounds, color.r, color.g, color.b, color.a - 200, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(drawBounds, color.r, color.g, color.b , color.a + 100, true, false);
		break;
	case GuiControlState::SELECTED:
		render->DrawRectangle(drawBounds, color.r, color.g, color.b, color.a + 100, true, false);
		break;
	}

	return false;
}

bool GuiButton::Deactivate()
{
	state = GuiControlState::DISABLED;

	return false;
}


