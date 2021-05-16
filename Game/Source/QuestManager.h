#ifndef __QUEST_MANAGER_H__
#define __QUEST_MANAGER_H__

#include "Module.h"
#include "List.h"
#include "SString.h"
#include "App.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Quest.h"

#include "External/PugiXml/src/pugixml.hpp"

#include <string>

using namespace std;

class Font;

class QuestManager : public Module
{
public:
	QuestManager();

	~QuestManager();

	// Called before the first frame
	bool Start();

	bool Update(float dt);

	bool PostUpdate(float dt);

	bool CleanUp();

	bool CheckQuestsLogic();
	bool CheckObjectivesCompletion();

	bool AcceptQuest(int id);
	bool AbandonQuest(int id);
	bool CompleteQuest(int id);

	bool DrawActiveQuests();

	void DrawQuestUi();

	void ShowQuestDescription();

	void CreateQuestButtons();

	void ExclamationDraw();

public:

	List<Quest*> questsList;
	List<Quest*> questsInactive;
	List<Quest*> questsActive;
	List<Quest*> questsFinished;

	//Quest Menu Properties
	SDL_Texture** customerTex;//Quest Menu Texture
	SDL_Texture** reaperTex;//Quest Menu Texture
	SDL_Texture** tavernTex;//Quest Menu Texture
	SDL_Texture** thymaTex;//Quest Menu Texture
	SDL_Texture** exclamation;

	List<GuiControl*> questButtons;//Buttons List - Need it, Don't delete.

	bool questInvOpened = false;//proof of whether it is open or not

	Font* font;

	bool selectPressed = true;

	int debugId = 1;
	int y = 0;

};


#endif
