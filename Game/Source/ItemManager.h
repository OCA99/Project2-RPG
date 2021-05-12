#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#include "Module.h"
#include "App.h"

#include "External/PugiXml/src/pugixml.hpp"
#include "SString.h"
#include "List.h"

#include <string>

using namespace std;

class Font;

class Item
{
public:
	Item() {};
	~Item() {};
public:
	int id; // Quest ID (unique for each quest)
	int type; // Quest type: 0 --> gather 1 --> kill 2 --> delivery 3 --> dialogue
	SString title; // Quest title
	SString description; // Quest description
	SString objective; // Object gathered or monster killer or NPC talked
	int quantity; // Amount of objective required for the quest
	SString demandingNPC; // Name of NPC quest giver
	SString rewardingNPC; // Name of rewarding NPC
	int rewardXP; // Quantity of XP rewarded
	int rewardGold; // Quantity of Gold rewarded
	int requiredId; // For a quest chain if it requires another quest ID completed before
	bool isCompleted; // true --> Quest completed // false --> Quest not completed
	int status; // 0 inactive, 1 active, 2 completed
};

class ItemManager : public Module
{
public:
	ItemManager();

	~ItemManager();

	// Called before the first frame
	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:

	//List<Quest*> questsList;
	//List<Quest*> questsInactive;
	//List<Quest*> questsActive;
	//List<Quest*> questsFinished;

	//Font* font;

	//int debugId = 1;
};


#endif
