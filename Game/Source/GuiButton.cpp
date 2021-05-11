#include "GuiButton.h"
#include "Window.h"
#include "App.h"
#include "Debug.h"
#include "Audio.h"
#include "SceneManager.h"


GuiButton::GuiButton(uint32 id, SDL_Rect bounds, SDL_Texture* tex, SDL_Texture* textTex) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->texture = tex;
	this->texture2 = textTex;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt)
{
	if (app->scene->graphicsSelected)
	{
		if (id == 12 || id == 13) state = GuiControlState::NORMAL;
		if(id == 11) state = GuiControlState::SELECTED;
	}

	if (app->scene->audioSelected)
	{
		if (id == 11 || id == 13) state = GuiControlState::NORMAL;
		if (id == 12) state = GuiControlState::SELECTED;
	}

	if (app->scene->controlsSelected)
	{
		if (id == 11 || id == 12) state = GuiControlState::NORMAL;
		if (id == 13) state = GuiControlState::SELECTED;
	}

	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)) &&  state != GuiControlState::SELECTED)
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
				state = GuiControlState::SELECTED;
				return NotifyObserver();
			}
		}
		else if (state != GuiControlState::SELECTED) state = GuiControlState::NORMAL;
		
	}
	return true;
}

bool GuiButton::Draw(Render* render)
{
	int scale = app->win->GetScale();

	SDL_Rect drawBounds = SDL_Rect({ bounds.x * scale, bounds.y * scale, bounds.w * scale, bounds.h * scale });
	// Draw the right button depending on state
	if (app->debug->bounds)
	{
		switch (state)
		{
		case GuiControlState::DISABLED:
			render->DrawRectangle(drawBounds, 255, 173, 173, 150, true, false);
			break;
		case GuiControlState::FOCUSED:
			render->DrawRectangle(drawBounds, 202, 255, 191, 150, true, false);
			break;
		case GuiControlState::NORMAL:
			render->DrawRectangle(drawBounds, 255, 173, 173, 150, true, false);
			break;
		case GuiControlState::PRESSED:
			render->DrawRectangle(drawBounds, 202, 255, 191, 150 + 100, true, false);
			break;
		case GuiControlState::SELECTED:
			render->DrawRectangle(drawBounds, 202, 255, 191, 150 + 100, true, false);
			break;
		}
	}
	else
	{
		switch (state)
		{
		case GuiControlState::DISABLED:
			break;
		case GuiControlState::FOCUSED:
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ drawBounds.w + 1,0,drawBounds.w + 1,drawBounds.h + 1}), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x, bounds.y, &SDL_Rect({ 0,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			break;
		case GuiControlState::NORMAL:
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x, bounds.y, &SDL_Rect({ 0,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			break;
		case GuiControlState::PRESSED:
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ (drawBounds.w + 1) * 2,0,drawBounds.w + 1,drawBounds.h + 1}), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x - 2, bounds.y + 1, &SDL_Rect({ 0,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			break;
		case GuiControlState::SELECTED:
			render->DrawTexture(texture2, bounds.x, bounds.y, &SDL_Rect({ 0,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			break;
		}

	}

	return false;
}

bool GuiButton::Deactivate()
{
	state = GuiControlState::DISABLED;

	return false;
}


