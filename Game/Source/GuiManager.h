#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

struct SDL_Texture;

class GuiManager : public Module
{
public:

	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	bool Update(float dt);

	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, SDL_Rect bounds, int id);
	void DestroyGuiControl(GuiControl* entity);

	void DestroyAllGuiControls();

	void AddGuiControl(GuiControl* entity);

	bool UpdateAll(float dt);

	void DrawAll();

public:

	List<GuiControl*> controls;
};

#endif // __GUIMANAGER_H__
