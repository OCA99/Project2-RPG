#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#include "Module.h"
#include "App.h"
#include "Textures.h"
#include "GuiManager.h"

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

	void ShowDescription();

	void CreateButtons();

public:
	SDL_Texture* invMenu;
	SDL_Texture* itemDescTex;

	List<Item*> itemList;//ALL Items
	List<Item*> playerItemList;//Player Items
	List<GuiControl*> buttons;//Buttons

	int y = 0;

	bool tempDraw = true;//ignore this
	bool invOpened = false;

};


#endif
