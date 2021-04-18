#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->checkedBounds = { bounds.x + 15, bounds.y + 15, 50, 50 };
	this->text = text;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt)
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
				checked = !checked;
				return NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return true;
}

bool GuiCheckBox::Draw(Render* render)
{
	switch (state)
	{
	case GuiControlState::DISABLED:
	{
		if (checked) render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);
		else render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);
	} break;
	case GuiControlState::NORMAL:
	{
		if (checked)
		{
			render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);//BG QUAD -
			render->DrawRectangle(checkedBounds, color.r, color.g, color.b, color.a, true, false);//LITTLE QUAD - 
		}
		else render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);
	} break;
	case GuiControlState::FOCUSED:
		if (checked)
		{
			render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);//IDLE
			render->DrawRectangle(checkedBounds, color.r, color.g, color.b, color.a, true, false);//YELLOW
		}
		else render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);;//YELLOW
		break;
	case GuiControlState::PRESSED:
		if (checked)
		{
			render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);//IDLE
			render->DrawRectangle(checkedBounds, color.r, color.g, color.b, color.a, true, false);//PURPLE
		}
		else render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);//PURPLE
		break;
	case GuiControlState::SELECTED: render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);
		break;
	default:
		break;
	}

	return false;
}

