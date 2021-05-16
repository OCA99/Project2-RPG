#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"
#include "Log.h"

#include "Point.h"
#include "SString.h"

class GuiCheckBox : public GuiControl
{
public:

	GuiCheckBox(uint32 id, SDL_Rect bounds, SDL_Texture** tex, SDL_Texture** textTex);
	virtual ~GuiCheckBox();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);

	bool checked;
	bool playFxOnce = true;

	bool aPressed = false;

private:
	SDL_Rect checkedBounds;
};

#endif // __GUICHECKBOX_H__

