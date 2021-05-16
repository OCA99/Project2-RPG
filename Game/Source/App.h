#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "Timer.h"

#include "PugiXml/src/pugixml.hpp"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class SceneManager;
class DialogSystem;
class Map;
class Fonts;
class ModuleUI;
class GuiManager;
class Debug;
class PartyManager;
class BattleManager;
class QuestManager;
class ItemManager;
class AssetsManager;
class PuzzleManager;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	void UpdateVolume();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();


public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	SceneManager* scene;
	DialogSystem* dialog;
	Map* map;
	Fonts* fonts;
	GuiManager* ui;
	Debug* debug;
	PartyManager* party;
	BattleManager* battle;
	ItemManager* items;
	QuestManager* quests;
	AssetsManager* assetsManager;
	PuzzleManager* puzzleManager;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	// TODO 2: Create new variables from pugui namespace:
	// a xml_document to store the config file and
	// two xml_node to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	float minTime;
	float frameCap = 60.0f;
	Timer timer;

	bool requestLoad = false;
	bool requestSave = false;

	const char* saveFileName;

public:
	float dt;
	float volume = 100;
	float volumeMusic = 100;
	float volumeFx = 100;
	bool volumeUp = false;
	bool volumeDown = false;

public:

	bool Load();
	bool Save();
	bool saving = false;

	void RequestSave()
	{
		requestSave = true;
	}

	void RequestLoad()
	{
		requestLoad = true;
	}
};

extern App* app;

#endif	// __APP_H__