#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Audio.h"

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, SDL_Texture** tex, int sliderValue);
	virtual ~GuiSlider();

	bool Start();
	bool Update(Input* input, float dt);
	bool Draw(Render* render);
	void SetValue(int v);

	int value;
	int sliderPosx;
	float unit;

	SDL_Rect slider;

	bool aPressed = false;
private:

	// GuiSlider specific properties
	// Maybe some animation properties for state change?
	int minValue;
	int maxValue;

};

#endif // __GUISLIDER_H__