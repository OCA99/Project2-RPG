#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Audio.h"

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiSlider();

	bool Start();
	bool Update(Input* input, float dt);
	bool Draw(Render* render);

	int value;
	SDL_Rect slider;
	int unit;
private:

	// GuiSlider specific properties
	// Maybe some animation properties for state change?
	int minValue;
	int maxValue;

};

#endif // __GUISLIDER_H__