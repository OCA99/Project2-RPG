#ifndef __QUEST_MANAGER_H__
#define __QUEST_MANAGER_H__

#include "Module.h"
#include "List.h"
#include "SString.h"
#include "App.h"
#include "Textures.h"
#include "GuiManager.h"

#include "External/PugiXml/src/pugixml.hpp"

#include <string>
using namespace std;

class Font;

class Quest
{
public:
	Quest() {};
	~Quest() {};
public:
	int id; // Quest ID (unique for each quest)
	int type; // Quest type: 0 --> gather 1 --> kill 2 --> delivery 3 --> dialogue
	SString title; // Quest title
	SString description; // Quest description
	SString objective; // Object gathered or monster killer or NPC talked
	int progress; //progress of the objective
	int quantity; // Amount of objective required for the quest
	SString demandingNPC; // Name of NPC quest giver
	SString rewardingNPC; // Name of rewarding NPC
	int rewardXP; // Quantity of XP rewarded
	int rewardGold; // Quantity of Gold rewarded
	int requiredId; // For a quest chain if it requires another quest ID completed before
	SString reward; //Reward Item
	bool isCompleted; // true --> Quest completed // false --> Quest not completed
	int status; // 0 inactive, 1 active, 2 completed
};

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

public:

	List<Quest*> questsList;
	List<Quest*> questsInactive;
	List<Quest*> questsActive;
	List<Quest*> questsFinished;

	//Quest Menu Properties
	SDL_Texture* questMenuTex;//Quest Menu Texture
	SDL_Texture* customerTex;//Quest Menu Texture
	SDL_Texture* reaperTex;//Quest Menu Texture
	SDL_Texture* tavernTex;//Quest Menu Texture
	SDL_Texture* thymaTex;//Quest Menu Texture

	List<GuiControl*> questButtons;//Buttons List - Need it, Don't delete.

	bool questInvOpened = false;//proof of whether it is open or not

	Font* font;

	int debugId = 1;
	int y = 0;
};


#endif
