#include "GuiButton.h"
#include "Window.h"
#include "App.h"
#include "Debug.h"
#include "Audio.h"
#include "SceneManager.h"
#include "GuiManager.h"
#include "ItemManager.h"
#include "QuestManager.h"
#include "PuzzleManager.h"
#include "Input.h"

#include "Log.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, SDL_Texture** tex, SDL_Texture** textTex) : GuiControl(GuiControlType::BUTTON, id)
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
		if (id == 11) state = GuiControlState::SELECTED;
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
	if (id == 17 && state == GuiControlState::FOCUSED)
	{
		if (!itemCheck) itemCheck = true;
	}
	if (id == 17 && state == GuiControlState::SELECTED)
	{
		for (int i = 0; i < app->items->buttons.Count(); ++i)
		{
			app->items->buttons[i]->itemSingleCheck = false;
		}
		app->items->DeleteActionButtons();
		if (!itemSingleCheck) itemSingleCheck = true;

		state = GuiControlState::NORMAL;
		
	}
	if (id == 17 && state != GuiControlState::FOCUSED && state != GuiControlState::SELECTED)
	{
		if (itemCheck) itemCheck = false;
	}
	if (id == 18 && state == GuiControlState::SELECTED)
	{
		for (int i = 0; i< app->quests->questButtons.Count() ; ++i)
		{
			app->quests->questButtons[i]->questCheck = false;
		}
	
		if (!questCheck) questCheck = true;
		state = GuiControlState::NORMAL;

	}
	if (id == 19 && state == GuiControlState::SELECTED)
	{

		if (!itemUsed) itemUsed = true;
		state = GuiControlState::NORMAL;

	}
	else
	{
		if (itemUsed) itemUsed = false;
	}
	if (id == 20 && state == GuiControlState::SELECTED)
	{
		if (!discarItem) discarItem = true;
		state = GuiControlState::NORMAL;

	}
	else
	{
		if (discarItem) discarItem = false;
	}
	if (id == 21 && state == GuiControlState::SELECTED)
	{
		for (int i = 0; i < app->puzzleManager->codeButtons.Count(); ++i)
		{
			app->puzzleManager->codeButtons[i]->checkPuzzle = false;
		}
		if (!checkPuzzle) checkPuzzle = true;
		state = GuiControlState::NORMAL;

	}



	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)) && state != GuiControlState::SELECTED)
		{
			state = GuiControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)	state = GuiControlState::PRESSED;

			if (app->input->pads[0].a)
			{
				aPressed = true;
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if ((input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || (!app->input->pads[0].a && aPressed)) )
			{
				aPressed = false;
				app->audio->PlayFx(6, 0);
				state = GuiControlState::SELECTED;
				if (id == 5 && state == GuiControlState::SELECTED) state = GuiControlState::NORMAL;
				return NotifyObserver();
			}

			if (!app->input->pads[0].a) aPressed = false;
		}
		else if (state != GuiControlState::SELECTED)
		{
			playFxOnce = true;
			state = GuiControlState::NORMAL;
		}

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
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ drawBounds.w + 1,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x, bounds.y, &SDL_Rect({ 0,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			if (app->ui->lastId != id) playFxOnce = true;

			if (playFxOnce)
			{
				app->audio->PlayFx(8, 0);
				playFxOnce = false;
				app->ui->lastId = id;
			}
			break;
		case GuiControlState::NORMAL:
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			render->DrawTexture(texture2, bounds.x, bounds.y, &SDL_Rect({ 0,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			break;
		case GuiControlState::PRESSED:
			render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ (drawBounds.w + 1) * 2,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			if (id == 14 || id == 19 || id == 20)
				render->DrawTexture(texture2, bounds.x, bounds.y, &SDL_Rect({ 0,0,drawBounds.w + 1,drawBounds.h + 1 }), .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
			else
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


