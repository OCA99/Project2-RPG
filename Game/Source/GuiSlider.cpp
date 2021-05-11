#include "GuiSlider.h"
#include "App.h"
#include "GuiManager.h"
#include "Window.h"
#include <stdio.h>      /* printf */
#include <math.h>       /* round, floor, ceil, trunc */

#include "Log.h"


GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, SDL_Texture* tex) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	this->texture = tex;
	slider = { bounds.x, bounds.y, 30, 30 };

	// el musicVolume iii fxVolume empieza en 100 en guiManager, no se cambia en ning�n lao, tmbi�n se tiene q hacer q cuando lo pruebes
	//suene al volumen q toca pq cuando estamos en menu la musica se capea a 25.
	if (id == 8)
	{
		value = app->ui->musicVolume;
	}
	else if (id == 9)
	{
		value = app->ui->fxVolume;
	}

	value = round(value);
	sliderPosx = ((value * unit) + bounds.x) - unit;
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
		if ((mouseX > bounds.x + 5) && (mouseX < (bounds.x + bounds.w) - 8) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			unit = bounds.w / 100.0f;
			value = (mouseX - bounds.x) / unit;
			value = round(value);

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				sliderPosx = ((value * unit) + bounds.x) - unit - 5;
			}

			// If mouse button pressed -> Generate event!
			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				state = GuiControlState::SELECTED;
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return true;
}

bool GuiSlider::Draw(Render* render)
{
	// Draw the right button depending on state
	int scale = app->win->GetScale();
	SDL_Rect drawBounds = SDL_Rect({ bounds.x * scale, bounds.y * scale, bounds.w * scale, bounds.h * scale });

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 0, 300, 30 }), 0.5f, 0, 0, 0, false);
		render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 300, 0, 30, 30 }), 0.5f, 0, 0, 0, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 0, 300, 30 }), 0.5f, 0, 0, 0, false);
		render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 330, 0, 30, 30 }), 0.5f, 0, 0, 0, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 0, 300, 30 }), 0.5f, 0, 0, 0, false);
		render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 300, 0, 30, 30 }), 0.5f, 0, 0, 0, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 0, 300, 30 }), 0.5f, 0, 0, 0, false);
		render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 360, 0, 30, 30 }), 0.5f, 0, 0, 0, false);
		break;
	case GuiControlState::SELECTED:
		render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 0, 300, 30 }), 0.5f, 0, 0, 0, false);
		render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 300, 0, 30, 30 }), 0.5f, 0, 0, 0, false);
		break;
	}

	return false;
}
