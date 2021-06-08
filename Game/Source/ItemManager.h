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

#define MAX_ITEMS 8

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
	SDL_Texture** itemTex = nullptr;
};

class ItemManager : public Module
{
public:
	ItemManager();

	~ItemManager();

	bool Start();

	bool Update(float dt);

	bool PostUpdate(float dt);

	bool CleanUp();

	//Draw Items Icon & Title
	void DrawPlayerItems();

	//Give item to a player, USE TITLE AS PARAMETER
	void GiveItemToPlayer(SString& itemTitle);

	//Search for an item BY ITS TITLE(NAME)
	Item* SearchForItem(SString& itemTitle); 

	//Draw the box with te Description Text
	void ShowDescription(); 

	//Show Item Actions
	void CreateActionButtons(int y);

	//Create Description & Exit Buttons
	void CreateButtons();

	void DeleteButtons();

	void DeleteActionButtons();

	void UseItem(Item* itemToUse, int y);

	void CheckActionButtons();

	void DrawPlayerStats();


public:
	SDL_Texture** itemDescTex;

	List<Item*> itemList;//ALL Items
	List<Item*> playerItemList;//Player Items
	List<GuiControl*> buttons;//Buttons List
	List<GuiControl*> actionButtons;//Buttons List

	int y = 0;//Logistic Tool

	bool invOpened = false;//True = Opened, False = closed.
	bool createButtons = true;
	bool partyMember = false; //false == toisto ;; true == thyma

	bool useItem = false;
	bool removeItem = false;
	bool yPressed = true;

	

};


#endif
