#pragma once

#include <map>
#include <string>
#include "Module.h"

class DialogNode;

class DialogSystem : public Module {
public:
	DialogSystem();

	// Destructor
	virtual ~DialogSystem();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool LoadDialog(const char* filename);

private:
	DialogNode* ParseDialogXML(pugi::xml_node dialogRoot);
	void PrintDialog(DialogNode* node);

private:
	std::map<std::string, DialogNode*> dialogues;

	std::string folder;
	pugi::xml_document dialogFile;
};