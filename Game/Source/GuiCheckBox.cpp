#include "GuiCheckBox.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Window.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, SDL_Texture* tex, SDL_Texture* textTex) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->checkedBounds = { bounds.x + 15, bounds.y + 15, 50, 50 };
	this->text = text;
	this->texture = tex;
	this->texture2 = textTex;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt)
{

	if (app->win->fullscreen && id == 15) checked = true;
	if (!app->win->fullscreen && id == 15) checked = false;

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
				app->audio->PlayFx(6, 0);
				checked = !checked;
				return NotifyObserver();
			}
		}
		else
		{
			playFxOnce = true;
			state = GuiControlState::NORMAL;
		}
	}

	return true;
}

bool GuiCheckBox::Draw(Render* render)
{
	switch (state)
	{
	case GuiControlState::DISABLED:
		break;
	case GuiControlState::NORMAL:
		if (checked)
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 0, 183, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x, bounds.y, &SDL_Rect({ 0, 0, 92, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		}
		else
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 100, 183, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x + 46, bounds.y, &SDL_Rect({ 92, 0, 91, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		}
		break;
	case GuiControlState::FOCUSED:
		if (app->ui->lastId != id) playFxOnce = true;

		if (playFxOnce)
		{
			app->audio->PlayFx(8, 0);
			playFxOnce = false;
			app->ui->lastId = id;
		}
		if (checked)
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 50, 183, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x, bounds.y, &SDL_Rect({ 0, 0, 92, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		}
		else
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 150, 183, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x + 46, bounds.y, &SDL_Rect({ 92, 0, 91, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		}
		break;
	case GuiControlState::PRESSED:
		if (checked)
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 50, 183, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x - 1, bounds.y + 1, &SDL_Rect({ 0, 0, 92, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		}
		else
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 150, 183, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x + 45, bounds.y + 1, &SDL_Rect({ 92, 0, 91, 50 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
		}
		break;
	case GuiControlState::SELECTED: render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);
		break;

	default:
		break;
	}

	return false;
}

