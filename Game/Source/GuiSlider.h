#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Audio.h"
#include "Easing.h"

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, SDL_Texture** tex, int sliderValue);
	GuiSlider(uint32 id, SDL_Rect bounds, SDL_Texture** tex, int sliderValue, bool horizontal, int fPos, float time, SplineType esType);

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