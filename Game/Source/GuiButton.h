#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, SDL_Texture* tex, SDL_Texture* textTex);
	virtual ~GuiButton();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);
	bool Deactivate();

	bool playFxOnce = true;

};

#endif // __GUIBUTTON_H__
