#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Point.h"

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
	Scene* sceneToBeLoaded = nullptr;
	fPoint playerPositionToBeLoaded;

	Scene* currentScene = nullptr;

	SDL_Texture* menuTex;

	SDL_Texture* pauseTex;
	SDL_Texture* continueTex;
	SDL_Texture* saveTex;
	SDL_Texture* loadTex;
	SDL_Texture* optionTex;
	SDL_Texture* mainTex;


	bool menu = false;
	bool buttons = false;

private:

	
};

#endif // __SCENE_H__