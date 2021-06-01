#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Point.h"
#include "List.h"

struct SDL_Texture;
class Scene;
class MapScene;
class GuiControl;

class SceneManager : public Module
{
public:

	SceneManager();

	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);
	
	// Called before quitting
	bool CleanUp();

	void LoadScene(Scene* scene);

	void LoadScene(MapScene* scene, fPoint playerPosition);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	Scene* currentScene = nullptr;
	Scene* sceneToBeLoaded = nullptr;
	fPoint playerPositionToBeLoaded;


	SDL_Texture** menuTex;
	SDL_Texture** optionsTex;
	SDL_Texture** graphicsMenuTex;
	SDL_Texture** audioMenuTex;
	SDL_Texture** questMenuTex;//Quest Menu Texture
	SDL_Texture** invMenu;

	bool graphicsSelected = false;
	bool audioSelected = false;
	bool controlsSelected = false;

	bool menu = false;
	bool optionsMenu = false;
	bool buttons = false;

	float alpha = 0;
	float speed = 250.0f;
	float volSpeed = 50.0f;
	bool isFinished = false;

	bool changingScene = false;

	bool startPressed;

	bool r1Pressed = true;
	bool l1Pressed = true;

	int padSpeed = 0;

private:

	
};

#endif // __SCENE_H__