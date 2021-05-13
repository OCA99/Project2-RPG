#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#include "Module.h"
#include "App.h"
#include "Textures.h"

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

	int type; // Item type: 0 --> weapon --> 1 --> armor --> 2 --> potion
	int quantity; // Amount of items in possession
	int buyCost; // Quantity of Gold item costs on buy
	int sellCost; // Quantity of Gold item costs on sell
	bool questItem; // True if it is for a quest
	SString title; // Item title
	SString description; // Item description
	SString objective; // target to the enemy who drops
	SString rewardingNPC; // Name of rewarding NPC
	SString texturePath; //Texture Path
	SDL_Texture* itemTex = nullptr;
};

class ItemManager : public Module
{
public:
	ItemManager();

	~ItemManager();

	// Called before the first frame
	bool Start();

	bool Update(float dt);

	bool PostUpdate(float dt);

	bool CleanUp();

	void DrawPlayerItems();

	void GiveItemToPlayer(SString& itemTitle) { playerItemList.Add(SearchForItem(itemTitle)); }

	Item* SearchForItem(SString& itemTitle);

	void CreateHovers();

public:
	SDL_Texture* invMenu;
	//List<Quest*> questsList;
	//List<Quest*> questsInactive;
	List<Item*> itemList;//ALL Items
	List<Item*> playerItemList;//Player Items
	bool tempDraw = true;//ignore this
	int y = 0;
	bool invOpened = false;
	//List<Quest*> questsActive;
	//List<Quest*> questsFinished;
};


#endif
